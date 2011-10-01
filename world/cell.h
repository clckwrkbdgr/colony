#ifndef CELL_H
#define CELL_H

#include "worldtypes.h"

class Creature;
class Map;

class Cell
{
	Sprite floorSprite; //sprite for floor for this cell
	Creature *creature;
	int _x,_y;
	Map *map;
public:
	Cell(Map *newMap,int newX,int newY,Sprite newFloorSprite,
		 Creature *newCreature=0);
	Cell(Cell &other);
	Cell& operator=(Cell &other);

	Creature* dweller();
	bool isEmpty();
	//this two func. is complete intersected:
	bool settle(Creature *newCreature); //settle creature to cell if empty
	bool moveDwellerTo(Cell *cell); //move dweller to new cell if it isn't empty

	int x();
	int y();
	Sprite sprite();
};

#endif // CELL_H
