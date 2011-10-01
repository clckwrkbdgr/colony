#include <stdlib.h>
#include <iostream>
#include "cell.h"
#include "creature.h"

Cell::Cell(Map *newMap,int newX,int newY,Sprite newFloorSprite,
		   Creature *newCreature)
{
	map=newMap;
	_x=newX;
	_y=newY;
	floorSprite=newFloorSprite;
	creature=newCreature;
}

Cell::Cell(Cell &other)
{
	map=other.map;
	_x=other._x;
	_y=other._y;
	floorSprite=other.floorSprite;
	creature=other.creature;
}

Cell& Cell::operator=(Cell &other)
{
	map=other.map;
	_x=other._x;
	_y=other._y;
	floorSprite=other.floorSprite;
	creature=other.creature;

	return *this;
}

Sprite Cell::sprite()
{
	return floorSprite;
}

Creature* Cell::dweller()
{
	return creature;
}

bool Cell::settle(Creature *newCreature)
{
	if(creature) return false; //already have a dweller
	if(!newCreature) return false; //a null dweller passed
	creature=newCreature;
	newCreature->changeCell(this); //просто сказать cr. поменять место.
	return true;
}

bool Cell::moveDwellerTo(Cell* cell)
{
	if(!cell) return false;
	if(cell==this) return false; //must not be an null movement
	if(!creature) return false; //must be a creature to move
	if(cell->creature) return false; //must not be a cr. in dest cell
	if(map!=cell->map) return false; //src&dest must be from the same map
	
	cell->creature=creature;
	creature=0;
	cell->creature->changeCell(cell); //просто сказать cr. поменять место.
	return true;
}

bool Cell::isEmpty()
{
	return (creature==0);
}

int Cell::x()
{
	return _x;
}

int Cell::y()
{
	return _y;
}
