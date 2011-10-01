#ifndef WORLDTYPES_H
#define WORLDTYPES_H

typedef int Sprite; //is just index in spritelist

//struct for cell sprites: cell and object sprites.
struct Sprites
{
	Sprite cell,object;
};

enum CreatureType {crCreature,crPlant,crAnimal};

//------------------------------ PROFILES --------------------------------------

class CreatureProfile //just a structure; no methods and properness check
{
protected:
	CreatureType type;
public:
	int maxAge;
	Sprite aliveSprite,deadSprite;

	CreatureProfile()
	{
		type=crCreature;
		maxAge=1;
		aliveSprite=-1;
		deadSprite=-1;
	}
	CreatureType classType() { return type; }
};

class AnimalProfile : public CreatureProfile
{
public:
	int getHungry,deadHungry;
	int getSleepy,deadSleepy;
	
	AnimalProfile() : CreatureProfile()
	{
		type=crAnimal;
		getHungry=0; deadHungry=1;
		getSleepy=0; deadSleepy=1;
	}
};

class PlantProfile : public CreatureProfile
{
public:
	int mass;
	
	PlantProfile() : CreatureProfile()
	{
		type=crPlant;
		mass=1;
	}
};

#endif // WORLDTYPES_H
