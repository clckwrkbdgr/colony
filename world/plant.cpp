#include <iostream>
#include <cstdlib>
#include "plant.h"

Plant::Plant(World *newWorld,Cell *newCell,PlantProfile &profile)
	: Creature(newWorld,newCell,profile)
{
	classType=crPlant;

	if(profile.mass<=0)
	{
		std::cerr<<"Plant: profile.mass <= 0\n";
		exit(1);
	}
	else
		mass=profile.mass;
}

Plant::Plant(Plant &other) : Creature(other)
{
	mass=other.mass;
}

Plant& Plant::operator=(Plant &other)
{
	Creature::operator=(other);

	mass=other.mass;

	return *this;
}

Plant::~Plant()
{

}

bool Plant::nextCycle()
{
	if(!Creature::nextCycle()) return false;

	//inner processes
	mass+=1; //mass grows														//mass grows

	return true;
}
