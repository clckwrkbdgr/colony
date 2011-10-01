#include <stdlib.h>
#include <iostream>
#include "world.h"
#include "cell.h"
#include "map.h"
#include "creature.h"
#include "plant.h"
#include "animal.h"
#include "scenario.h"

World::World(Scenario& scenario)
{
	map=new Map(scenario);
	if(!map)
	{
		std::cerr<<"World: invalid map\n";
		exit(1);
	}

	list=new List<Creature*>;
	if(!list)
	{
		std::cerr<<"World: error malloc: list\n";
		exit(1);
	}

	if(!scenario.settleWorld(this,*list,*map))
	{
		std::cerr<<"World: can't settle world with scenario\n";
		exit(1);
	}

	dlink=new int;
	if(!dlink)
	{
		std::cerr<<"World: error malloc: dlink\n";
		exit(1);
	}
	(*dlink)++;
}

World::World(World &other)
{
	map=other.map;
	list=other.list;
	dlink=other.dlink;
	(*dlink)++;
}

World& World::operator=(World &other)
{
	//destroy
	(*dlink)--;
	if((*dlink)==0)
	{
		delete map;
		delete list;
		delete dlink;
	}

	//construct
	map=other.map;
	list=other.list;
	dlink=other.dlink;
	(*dlink)++;

	return *this;
}

World::~World()
{
	(*dlink)--;
	if((*dlink)==0)
	{
		delete map;
		delete list;
		delete dlink;
	}
}

int World::mapWidth()
{
	return map->mapWidth();
}

int World::mapHeight()
{
	return map->mapHeight();
}

void World::nextCycle()
{
	//tells every Creature to live their own lifecycle
	list->toStart();
	while(!list->atEnd())
		list->next()->nextCycle();
}

Sprite World::sprite(int x,int y)
{
	return map->sprite(x,y);
}

bool World::getStep(Cell *src,Cell *dest)
{
	if(src==dest)
		return false; //invalid movement
	
	List<Cell*> list=map->neighbours(src,true);
	int freeCount=list.count();
	if(freeCount==0) return false;
	
	list.toStart();
	while(!list.atEnd())
	{
		Cell *cell=list.next();
		if(map->distance(*cell,*dest)<map->distance(*src,*dest))
			dest=cell;
	}
	
	//move Creature to new pos
	return src->dweller()->changeCell(dest);
}

Cell* World::getRandNearbyCell(Cell *src)
{
	List<Cell*> list=map->neighbours(src,true);
	int freeCount=list.count();
	if(freeCount==0) return src;
	
	return list.at(rand()%freeCount);
}

bool World::isNeighbours(Cell &a,Cell &b)
{
	return (map->distance(a,b)==1);
}

Cell* World::getNearestPlant(Cell &cell)
{
	return map->getNearestObject(cell,crPlant);
}

