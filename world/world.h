#ifndef WORLD_H
#define WORLD_H

#include "dumblist.h"
#include "worldtypes.h"

class Creature;
class Cell;
class Map;
class Scenario;

class World //incapsulates whole world with map and list of creatures
{
	Map *map;
	List<Creature*> *list;

	int *dlink;
public:
	World(Scenario& scenario); //creates world by scenario
	World(World &other);
	World& operator=(World &other);
	~World();

	int mapWidth();
	int mapHeight();

	void nextCycle(); //make all Creatures lives their own lifecycle
	Sprite sprite(int x,int y); //output current view of world

	//tries to move creature from src to dest; in fact, gets the first step of..
	bool getStep(Cell *src,Cell *dest); //..path src->dest and move cr. to it
	Cell* getRandNearbyCell(Cell *src); //(means empty) returns src if failed
	Cell* getNearestPlant(Cell &cell); //return the nearest to cell plant
	bool isNeighbours(Cell &a,Cell &b); //are this two cell neighbours?
};

#endif // WORLD_H
