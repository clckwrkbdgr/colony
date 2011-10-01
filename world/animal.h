#ifndef ANIMAL_H
#define ANIMAL_H

#include "creature.h"

class Animal : public Creature //animal - moves, eats, sleeps
{
protected:
	int sleepiness,getSleepy,deadSleepy; //0 - not tired; >=getX - gets tired;
	int hunger,getHungry,deadHungry; //>=deadX - dead.

	Cell *target; //target for moving to [eat]
public:
	//not sleepy,not tired and has no target to eat
	Animal(World *newWorld,Cell *newCell,AnimalProfile &profile);
	Animal(Animal &other);
	Animal& operator=(Animal &other);
	virtual ~Animal();

	bool sleepy();
	bool hungry();

	virtual bool nextCycle(); //do hunger,sleep,moves - a BEHAVIOR
};

#endif // ANIMAL_H
