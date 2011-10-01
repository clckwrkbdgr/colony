#include <iostream>
#include <cstdlib>
#include "cell.h"
#include "animal.h"

Animal::Animal(World *newWorld,Cell *newCell,AnimalProfile &profile)
	: Creature(newWorld,newCell,profile)
{
	classType=crAnimal;

	sleepiness=0; //not sleepy
	hunger=0; //not tired
	target=0; //has no target to eat

	if(profile.getHungry<0)
	{
		std::cerr<<"Animal: profile.getHungry < 0\n";
		exit(1);
	}
	getHungry=profile.getHungry;
	if(profile.getHungry>=profile.deadHungry)
	{
		std::cerr<<"Animal: profile.getHungry >= profile.deadHungry\n";
		exit(1);
	}
	deadHungry=profile.deadHungry;
	if(profile.getSleepy<0)
	{
		std::cerr<<"Animal: profile.getSleepy < 0\n";
		exit(1);
	}
	getSleepy=profile.getSleepy;
	if(profile.getSleepy>=profile.deadSleepy)
	{
		std::cerr<<"Animal: profile.getSleepy >= profile.deadSleepy";
		exit(1);
	}
	deadSleepy=profile.deadSleepy;
}

Animal::Animal(Animal &other) : Creature(other)
{
	sleepiness=other.sleepiness;
	hunger=other.hunger;
	target=other.target;

	getHungry=other.getHungry;
	deadHungry=other.deadHungry;
	getSleepy=other.getSleepy;
	deadSleepy=other.deadSleepy;
}

Animal& Animal::operator=(Animal &other)
{
	Creature::operator=(other);

	sleepiness=other.sleepiness;
	hunger=other.hunger;
	target=other.target;

	getHungry=other.getHungry;
	deadHungry=other.deadHungry;
	getSleepy=other.getSleepy;
	deadSleepy=other.deadSleepy;
	return *this;
}

Animal::~Animal()
{

}

bool Animal::nextCycle()
{
	if(!Creature::nextCycle()) return false; //does whatever Creature does

	//inner processes
	hunger++;
	sleepiness++;

	//if too hungry or too sleepy then dies
	if(hunger>=deadHungry || sleepiness>=deadSleepy)
	{
		die();
		return false;
	}

	//if sleepy, goes to sleep right in the place
	if(sleepy() && !hungry())
	{
		sleepiness=0; //less the tire to "untired"
		return true;
	}

	//if hungry, goes to nearest Plant and eats it
	if(hungry())
	{
		//ask world about the nearest plant and sets the TARGET
		if(!target) target=world->getNearestPlant(*cell);
		//meet eating itself
		if(target) //if we've found target at all, or else go further
		{
			if(world->isNeighbours(*cell,*target)) //if plant is near - eat it
			{
				if(target->dweller()->die()) //tells eated Plant that it is dead
				{
					hunger=0; //reset the hunger to "not hungry at all"
					target=0; //and cr. has target no more
				}
			}
			else
				world->getStep(cell,target); //go to it by move to its direction
			return true;
		}
	}

	//like 'default' section in 'switch' operator:
	changeCell(world->getRandNearbyCell(cell)); //get random shuffle

	return true;
}

bool Animal::sleepy()
{
	return sleepiness>=getSleepy;
}

bool Animal::hungry()
{
	return hunger>=getHungry;
}
