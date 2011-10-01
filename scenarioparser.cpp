#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include "scenarioparser.h"

#include "../world/worldtypes.h"
#include "../world/scenario.h"
#include "../world/creature.h"
#include "../world/plant.h"
#include "../world/animal.h"
#include "../world/map.h"

ScenarioParser::ScenarioParser(QDir scenarioDir)
{
	//set given scenarioDir as current dir (and save the old one)
	QDir oldDir=QDir::current();
	if(!QDir::setCurrent(scenarioDir.absolutePath()))
	{
		qCritical()<<"Can't set dir"<<scenarioDir.absolutePath()<<"as current";
		exit(1);
	}

	//validates file name and opens file for read
	QString fileName="scenario"; //straight
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly | QFile::Text))
	{
		qCritical()<<"Can't open file "<<fileName<<" for read";
		exit(1);
	}
	QTextStream in(&file);

	//for profiles: must be synchronous! or else would be shit
	bool mapExists=false; //was map already declared
	QStringList profileFileList; //list of names
	QList<CreatureProfile*> profileList; //list of profiles

	//here is main read cycle
	while (!in.atEnd())
	{
		//get another line: split it into list of words
		QStringList list=in.readLine().simplified().split(' ');
		if(list.value(0).isEmpty()) continue; //empty lines has no effect
		
		//----------------- here is the syntax parser itself -------------------
		
		//check first word in the line
		if(list[0]=="floor")
		{
			if(addSprite(list.value(1)/*,false*/) < 0)
				qCritical()<<"Can't load floor sprite from '"<<list.value(1)<<"'";
			continue;
		}
		
		if(list[0]=="map")
		{
			if(mapExists)
			{
				qCritical()<<"Multiple declaration of map in scenario "<<fileName;
				exit(1);
			}
			QString widthString=list.value(1);
			QString heightString=list.value(2);
			
			bool ok=true;
			int mapWidth=widthString.toInt(&ok);
			if(!ok)
			{
				qCritical()<<"None-int map width in scenario "<<fileName;
				exit(1);
			}
			int mapHeight=heightString.toInt(&ok);
			if(!ok)
			{
				qCritical()<<"None-int map height in scenario "<<fileName;
				exit(1);
			}
			//here the map
			scenario.setMapWidth(mapWidth);
			scenario.setMapHeight(mapHeight);
			mapExists=true;
		}
		
		if(list[0]=="instance")
		{
			QString profileString=list.value(1);
			QString amountString=list.value(2);
			
			//all other lines will consider as new instances
			int profileIndex=profileFileList.indexOf(profileString);
			if(profileIndex<0) //if not contains, then load
			{
				//создал? удали
				CreatureProfile* profile=loadProfile(profileString);
				
				//add profile
				scenario.addProfile(profile);
				
				profileFileList.append(profileString);
				profileIndex=profileFileList.indexOf(profileString);
				delete profile; //а по-другому никак.
			}
 
			if(amountString.isEmpty())
				qWarning()<<"Empty amount value in instance";
			else
			{
				bool ok=true;
				int amount=amountString.toInt(&ok);
				if(!ok)
					qWarning()<<"None-int amount value in instance:"<<
							amountString;
				
				//add instance
				if(!scenario.addInstance(profileIndex,amount))
					qCritical()<<"Adding instance to scenario failed";
			}
			continue;
		}
	}
	if(!mapExists)
	{
		qCritical()<<"No map declared in scenario "<<fileName;
		exit(1);
	}
}

QList<QPixmap>& ScenarioParser::getSpriteList()
{
	return spriteList;
}

Scenario& ScenarioParser::getScenario()
{
	return scenario;
}

int ScenarioParser::addSprite(QString fileName/*,bool makeMask*/)
{
	//validate
	if(fileName.isEmpty())
	{
		qWarning()<<"Cannot load empty sprite";
		return -1;
	}
	
	//load
	int index=spriteFileList.indexOf(fileName); //if thereisn't such..
	if(index<0) //..sprite yet, load it now and index is it
	{
		//check and load
		QPixmap sprite;
		if(!sprite.load(fileName))
		{
			qCritical()<<"Failed to load sprite "<<fileName;
			return -1;
		}
		
		//add to lists
		spriteList<<sprite; //..value
		spriteFileList.append(fileName); //..name
		index=spriteFileList.indexOf(fileName);
		
		//if(makeMask) //make sprite transparent
			//spriteList[index].setMask(spriteList[index].createHeuristicMask());
	}
	return index;
}

bool ScenarioParser::readIntValue(QStringList list,QString name,int &value)
{
	if(list.isEmpty()) return false;

	bool ok=false;
	if(list[0]==name)
	{
		value=list.value(1).toInt(&ok);
		if(!ok)
			qWarning()<<"Value for property "<<name<<" is not integer;"
					" use default value (0)";
	}
	return ok;
}

bool ScenarioParser::readSprite(QStringList list,QString name,int &spriteIndex)
{
	if(list.isEmpty()) return false;

	bool ok=false;
	if(list[0]==name)
	{
		spriteIndex=addSprite(list.value(1)/*,true*/);
		ok=(spriteIndex >= 0);
		if(!ok)
			qWarning()<<"Value for property "<<name<<":"<<list.value(1)<<
					" is not sprite file name; sprite index is -1";
	}
	return ok;
}

CreatureProfile* ScenarioParser::loadProfile(QString fileName)
{
	//open profile file for read
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly | QFile::Text))
	{
		qCritical()<<"Can't open file "<<fileName<<" for read";
		return false;
	}
	QTextStream in(&file);
	
	QStringList classNames;
	classNames<<"creature"<<"plant"<<"animal";

	//first line is name of class (profile type)
	QString className=in.readLine().simplified();
	
	if(!classNames.contains(className))
	{
		qWarning()<<"unknown profile type "<<className<<" in file "<<fileName;
		return 0;
	}
	
	CreatureType classType=(CreatureType)classNames.indexOf(className);
	CreatureProfile* profile=0;
	switch(classType)
	{
		case crCreature: profile=new CreatureProfile; break;
		case crPlant: profile=new PlantProfile; break;
		case crAnimal: profile=new AnimalProfile; break;
	}

	//all other the lines is profile properties formatted as: "name value" 
	while (!in.atEnd())
	{
		//get another line: split it into list of words
		QStringList list=in.readLine().simplified().split(' ');
		if(list.value(0).isEmpty()) continue; //skip empty line
		
		//read properties
		bool ok=false;
		//general properties
		ok=readIntValue(list,"maxAge",profile->maxAge) || ok;
		ok=readSprite(list,"aliveSprite",profile->aliveSprite) || ok;
		ok=readSprite(list,"deadSprite",profile->deadSprite) || ok;
		//private properties
		if(classType==crPlant)
		{
			PlantProfile* plant=(PlantProfile*)profile;
			ok=readIntValue(list,"mass",plant->mass) || ok;
		}
		if(classType==crAnimal)
		{
			AnimalProfile* animal=(AnimalProfile*)profile;
			ok=readIntValue(list,"deadHungry",animal->deadHungry) || ok;
			ok=readIntValue(list,"deadSleepy",animal->deadSleepy) || ok;
			ok=readIntValue(list,"getHungry",animal->getHungry) || ok;
			ok=readIntValue(list,"getSleepy",animal->getSleepy) || ok;
		}
		
		//check reading
		if(!ok)
			qWarning()<<"In profile "<<fileName<<": unknown property or parsing "
					"failed: "<<list[0];
	}

	return profile;
	
}
