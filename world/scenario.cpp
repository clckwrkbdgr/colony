#include <iostream>
#include "scenario.h"
#include "creature.h"
#include "plant.h"
#include "animal.h"
#include "map.h"
#include "cell.h"

Scenario::Scenario()
{
	_mapWidth=1;
	_mapHeight=1;

	dlink=new int;
	(*dlink)=1;
}

Scenario::Scenario(Scenario& other)
{
	_mapWidth=other._mapWidth;
	_mapHeight=other._mapHeight;
	profiles=other.profiles;
	instances=other.instances;
	
	dlink=other.dlink;
	(*dlink++);
}

Scenario& Scenario::operator=(Scenario& other)
{
	//destroy
	(*dlink)--;
	if(dlink<=0)
	{
		profiles.toStart();
		while(!profiles.atEnd())
			delete profiles.next();
	}

	//create & copy
	_mapWidth=other._mapWidth;
	_mapHeight=other._mapHeight;
	profiles=other.profiles;
	instances=other.instances;

	dlink=other.dlink;
	(*dlink++);
	
	return *this;
}

Scenario::~Scenario()
{
	(*dlink)--;
	if(dlink<=0)
	{
		profiles.toStart();
		while(!profiles.atEnd())
			delete profiles.next();
	}
}

int Scenario::mapWidth()
{
	return _mapWidth;
}

int Scenario::mapHeight()
{
	return _mapHeight;
}

bool Scenario::setMapWidth(int newValue)
{
	if(newValue>0)
	{
		_mapWidth=newValue;
		return true;
	}
	else
		return false;
}

bool Scenario::setMapHeight(int newValue)
{
	if(newValue>0)
	{
		_mapHeight=newValue;
		return true;
	}
	else
		return false;
}

bool Scenario::addProfile(CreatureProfile *profile)
{
	CreatureProfile *newProfile=0;
	//черт бы побрал цпп-шное кастование типов
	switch(profile->classType())
	{
		case crPlant:
		{
			newProfile=new PlantProfile;
			(*((PlantProfile*)newProfile))=(*((PlantProfile*)profile));
			break;
		}
		case crAnimal:
		{
			newProfile=new AnimalProfile;
			(*((AnimalProfile*)newProfile))=(*((AnimalProfile*)profile));
			break;
		}
		default: newProfile=new CreatureProfile;
		{
			newProfile=new CreatureProfile;
			(*((CreatureProfile*)newProfile))=(*((CreatureProfile*)profile));
			break;
		}
	}
	
	return profiles.append(newProfile);
}

bool Scenario::addInstance(int profile,int amount)
{
	Instance newInstance;
	if(profile>=profiles.count() || profile<0)
	{
		std::cerr<<"Scenario: addInstance("<<profile<<','
				<<amount<<"): profile < 0 or profile > profile list count\n";
		return false;
	}
	newInstance.profile=profile;
	if(amount<=0)
	{
		std::cerr<<"Scenario: addInstance("<<profile<<','
				<<amount<<"): amount <= 0\n";
		return false;
	}
	newInstance.amount=amount;
	return instances.append(newInstance);
}

bool Scenario::settleWorld(World* world,List<Creature*>& list,Map& map)
{
	//validate
	if(list.count()>0)
	{
		std::cerr<<"Scenario: world's list must be empty\n";
		return false;
	}

	if(!map.isEmpty()) //map is not empty
	{
		std::cerr<<"Scenario: world's map must be empty\n";
		return false;
	}

	//area counts
	int totalArea=map.mapWidth()*map.mapHeight(); //место для расселения
	int occupiedArea=0; //занятое место

	//let the filling begin
	instances.toStart();
	while(!instances.atEnd())
	{
		Instance instance=instances.next();
		for(int i=0;i<instance.amount;i++) //specified amount..
		{
			switch(profiles.at(instance.profile)->classType())
			{
				case crPlant:
					if(!list.append(new Plant(world,map.randomCell(),
						*((PlantProfile*) profiles.at(instance.profile))
						)))
						return false;
					break;
				case crAnimal:
					if(!list.append(new Animal(world,map.randomCell(),
						*((AnimalProfile*) profiles.at(instance.profile))
						)))
						return false;
					break;
				default:
					if(!list.append(new Creature(world,map.randomCell(),
						*(profiles.at(instance.profile))
						)))
						return false;
			}

			occupiedArea++; //занята еще одна ячейка
			if(occupiedArea>totalArea)
			{
				std::cerr<<"Scenario: overloading of map: too much creatures\n";
				return false;
			}
		}
	}
	return true;
}

