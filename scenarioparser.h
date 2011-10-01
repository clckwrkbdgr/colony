#ifndef SCENARIOPARSER_H
#define SCENARIOPARSER_H

#include <QWidget>
#include <QDir>
#include "../world/scenario.h"

class CreatureProfile;

class ScenarioParser
{
	QDir dir; //default dir for all file-operations such as load sprites etc

	Scenario scenario;

	QList<QPixmap> spriteList;
	
	QStringList spriteFileList;
	
	//adds sprite and returns index (or just returns index if sprite present)
	int addSprite(QString fileName);
	//load profile from file
	CreatureProfile* loadProfile(QString fileName);
	//read property as name=value, and return value as link
	bool readIntValue(QStringList list,QString name,int &value);
	bool readSprite(QStringList list,QString name,int &spriteIndex);
public:
	ScenarioParser(QDir scenarioDir);
	
	Scenario& getScenario();
	QList<QPixmap>& getSpriteList();
};

#endif // SCENARIOPARSER_H
