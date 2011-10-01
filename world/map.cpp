#include <stdlib.h>
#include <iostream>
#include "map.h"
#include "cell.h"
#include "creature.h"
#include "scenario.h"

Map::Map(Scenario &scenario)
{
	if(scenario.mapWidth()<1 || scenario.mapHeight()<1)
	{
		std::cerr<<"Map: invalid map size\n";
		exit(1);
	}

	width=scenario.mapWidth();
	height=scenario.mapHeight();

	//creates map of given size
	map=new Cell**[width];
	if(!map)
	{
		std::cerr<<"Map: error malloc: map\n";
		exit(1);
	}
	for(int i=0;i<width;i++)
	{
		map[i]=new Cell*[height];
		if(!map[i])
		{
			std::cerr<<"Map: error malloc: map[i]\n";
			exit(1);
		}
		for(int j=0;j<height;j++)
			map[i][j]=new Cell(this,i,j,0,0); //dead empbryonic cells			//cellsprite
	}

	dlink=new int;
	if(!dlink)
	{
		std::cerr<<"Map: error malloc: dlink\n";
		exit(1);
	}
	(*dlink)++;
}

Map::Map(Map &other)
{
	width=other.width;
	height=other.height;
	map=other.map;
	dlink=other.dlink;
	(*dlink)++;
}

Map& Map::operator=(Map &other)
{
	//destroy
	(*dlink)--;
	if((*dlink)==0)
	{
		//clear old world
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
				delete map[i][j];
			delete []map[i];
		}
		delete []map;
		delete dlink;
	}

	//construct
	width=other.width;
	height=other.height;
	map=other.map;
	dlink=other.dlink;
	(*dlink)++;

	return *this;
}

Map::~Map()
{
	(*dlink)--;
	if((*dlink)==0)
	{
		//clear old world
		for(int i=0;i<width;i++)
		{
			for(int j=0;j<height;j++)
				delete map[i][j];
			delete []map[i];
		}
		delete []map;
		delete dlink;
	}
}

int Map::mapWidth()
{
	return width;
}

int Map::mapHeight()
{
	return height;
}

Sprite Map::sprite(int x,int y)
{
	if(validCoords(x,y) && !map[x][y]->isEmpty())
		return map[x][y]->dweller()->sprite();
	else
		return -1;
}

bool Map::isEmpty()
{
	bool empty=true;
	for(int x=0;x<width;x++)
	{
		for(int y=0;y<height;y++)
			if(!map[x][y]->isEmpty())
			{
				empty=false;
				break;
			}
		if(!empty) break;
	}
	return empty;
}

int Map::distance(Cell &a,Cell &b)
{
	if(abs(a.x()-b.x())>abs(a.y()-b.y()))
		return abs(a.x()-b.x());
	else
		return abs(a.y()-b.y());
}

Cell* Map::randomCell()
{
	int x=rand()%width,y=rand()%height;
	while(!map[x][y]->isEmpty()) //look for an empty cell
	{
		x=rand()%width;
		y=rand()%height;
	}
	return map[x][y];
}

bool Map::validCoords(int x,int y)
{
	return (x>=0 && x<width && y>=0 && y<height);
}

List<Cell*> Map::neighbours(Cell *value,bool freeOnly)
{
	int cellX=value->x(),cellY=value->y();
	List<Cell*> list;
	
	//gets all the nearby cells (if exists)
	for(int x=cellX-1;x<=cellX+1;x++)
	{
		for(int y=cellY-1;y<=cellY+1;y++)
		{
			//if cell is valid and is not the value
			if(validCoords(x,y) && x!=cellX && y!=cellY)
			{
				if(!freeOnly || map[x][y]->isEmpty())
					list.append(map[x][y]);
			}
		}
	}
	return list;
}

Cell* Map::getNearestObject(Cell &cell,CreatureType type)
{
	Cell *object=0;
	//search through all cells for the nearest object
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			if(!map[i][j]->isEmpty()) //if there is an object
				if(map[i][j]->dweller()->type()==type && //if object has type
				   map[i][j]->dweller()->isAlive()) //that we need and its alive
					if(!object || //there is comparison of distances
					   distance(cell,*map[i][j])<distance(cell,*object))
						object=map[i][j];
	//send back object's cell of null if we hadn't find anything at all
	return object;
}

