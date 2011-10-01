#ifndef CREATURE_H
#define CREATURE_H

#include "worldtypes.h"
#include "world.h"

class Creature //basic creature class - only lives and withers and has viewform
{
protected:
	CreatureType classType;
	World *world;
	Cell *cell; //parent cell (ex- parent world and position on map)

	int age,maxAge;
	bool alive; //alive or not
	Sprite aliveSprite,deadSprite;
public:
	//creates Creature and sets it's age=0,alive=true
	Creature(World *newWorld,Cell *newCell,CreatureProfile &profile);
	Creature(Creature &other);
	Creature& operator=(Creature &other);
	virtual ~Creature();

	CreatureType type();
	bool isAlive();
	Sprite sprite();

	virtual bool nextCycle(); //do behave; return isAlive()

	//change cur.cell to dest and tell world itself by cell's help that this..
	bool changeCell(Cell *dest); //..creature has changed its position.
	bool die(); //becomes not alive (if already is, returns false)
};

#endif // CREATURE_H
