#include <stdlib.h>
#include <iostream>
#include "cell.h"
#include "creature.h"

Creature::Creature(World *newWorld,Cell *newCell,CreatureProfile &profile)
{
	if(!newWorld)
	{
		std::cerr<<"Creature: wrong 'world' arg\n";
		exit(1);
	}
	world=newWorld;
	if(!newCell)
	{
		std::cerr<<"Creature: wrong 'cell' arg\n";
		exit(1);
	}
	cell=newCell;
	if(!cell->settle(this)) //settles creature to cell, RIGHT HERE!
	{
		std::cerr<<"Creature: can't settle creature in the cell\n";
		exit(1);
	}

	classType=crCreature;
	age=0;
	alive=true;

	if(profile.maxAge<0)
	{
		std::cerr<<"Creature: profile.maxAge < 0\n";
		exit(1);
	}
	maxAge=profile.maxAge;
	if(profile.aliveSprite<0)
	{
		std::cerr<<"Creature: profile.aliveSprite < 0\n";
		exit(1);
	}
	aliveSprite=profile.aliveSprite;
	if(profile.deadSprite<0)
	{
		std::cerr<<"Creature: profile.deadSprite < 0\n";
		exit(1);
	}
	deadSprite=profile.deadSprite;
}

Creature::Creature(Creature &other)
{
	world=other.world;
	cell=other.cell;

	classType=other.classType;
	age=other.age;
	alive=other.alive;

	maxAge=other.maxAge;
	aliveSprite=other.aliveSprite;
	aliveSprite=other.deadSprite;
}

Creature& Creature::operator=(Creature &other)
{
	world=other.world;
	cell=other.cell;

	classType=other.classType;
	age=other.age;
	alive=other.alive;

	maxAge=other.maxAge;
	aliveSprite=other.aliveSprite;
	aliveSprite=other.deadSprite;

	return *this;
}

Creature::~Creature()
{

}

CreatureType Creature::type()
{
	return classType;
}

bool Creature::isAlive()
{
	return alive;
}

Sprite Creature::sprite()
{
	if(alive)
		return aliveSprite;
	else
		return deadSprite;
}

bool Creature::changeCell(Cell *dest)
{
	//при первом вызове это не так, но при втором, внутреннем, это уже так -
	//т.е. просто довершить работу и выйти с чистой совестью
	if(cell->dweller()!=this && dest->dweller()==this)
	{
		cell=dest;
		return true;
	}

	//сюда доходит только при первом вызове
	return cell->moveDwellerTo(dest);
}

bool Creature::nextCycle()
{
	if(!alive) return false;

	//inner processes
	age++;

	//gets older until die
	if(age>=maxAge)
	{
		die();
		return false;
	}
	return true;
}

bool Creature::die()
{
	if(alive)
	{
		alive=false; //becomes not alive
		return true;
	}
	return false;
}

