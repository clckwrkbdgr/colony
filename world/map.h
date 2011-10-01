#ifndef MAP_H
#define MAP_H

#include "worldtypes.h"
#include "dumblist.h"

class Cell;
class Scenario;

class Map
{
	//map (with size)
	Cell* **map;
	int width,height;

	int *dlink;

	bool validCoords(int x,int y);
public:
	Map(Scenario &scenario); //every dim. must be >= 1
	Map(Map &other);
	Map& operator=(Map&);
	~Map();

	int mapWidth();
	int mapHeight();
	Sprite sprite(int x,int y); //спрайт ячейки и ее содержимого по коориднатам

	bool isEmpty(); //is entire map empty
	
	List<Cell*> neighbours(Cell *cell,bool freeOnly); //список соседних cells
	Cell* randomCell(); //get random empty cell on whole map or null if invalid
	int distance(Cell &a,Cell &b); //dist between two cells (calc. as I2-path)
	//returns the nearest to cell object of type or null if invalid
	Cell* getNearestObject(Cell &cell,CreatureType type);
};

#endif // MAP_H
