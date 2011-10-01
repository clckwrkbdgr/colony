#ifndef PLANT_H
#define PLANT_H

#include "creature.h"

class Plant : public Creature //plant - like Creature but looks diff.
{
protected:
	int mass; //mass of food in plant
public:
	Plant(World *newWorld,Cell *newCell,PlantProfile &profile);
	Plant(Plant &other);
	Plant& operator=(Plant &other);
	virtual ~Plant();

	virtual bool nextCycle(); //does that Creature does, but looks as Plant
};

#endif // PLANT_H
