#ifndef SCENARIO_H
#define SCENARIO_H

#include "dumblist.h"
#include "worldtypes.h"

class CreatureProfile;
class Creature;
class Map;
class World;

class Scenario
{
	class Instance
	{
	public:
		int profile;
		int amount;
	};

	List<CreatureProfile*> profiles;
	List<Instance> instances;
	int _mapWidth,_mapHeight;
	
	int *dlink;
public:
	Scenario();
	Scenario(Scenario& other);
	Scenario& operator=(Scenario& other);
	~Scenario();

	int mapWidth();
	int mapHeight();

	bool addProfile(CreatureProfile *profile);
	bool addInstance(int profile,int amount);
	bool setMapWidth(int newValue);
	bool setMapHeight(int newValue);

	bool settleWorld(World *world,List<Creature*>& list,Map& map);
};

#endif // SCENARIO_H
