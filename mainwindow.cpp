#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QBitmap>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QKeyEvent>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "scenarioparser.h"

#include "../world/worldtypes.h"
#include "../world/map.h"
#include "../world/scenario.h"
#include "../world/animal.h"
#include "../world/plant.h"

#define INIT_INTERVAL 512
#define MIN_INTERVAL 1
#define MAX_INTERVAL 65536

#define CELL_WIDTH 32
#define CELL_HEIGHT 32
#define FLOOR_SPRITE 0

#define SCEN_DIR "sheeps/"

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	//timer
	cycleCount=0;
	timer.setInterval(INIT_INTERVAL);
	connect(&timer,SIGNAL(timeout()), this,SLOT(nextCycle()));
	
	//scenario and world
	ScenarioParser parser(QDir(SCEN_DIR));
	spriteList=parser.getSpriteList();
	world=new World(parser.getScenario());
}

MainWindow::~MainWindow()
{
	delete world;
}

void MainWindow::nextCycle()
{
	if(world)
	{
		world->nextCycle();
		
		//update the display
		cycleCount++;
		update();
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	switch(event->key())
	{
		case Qt::Key_Space:
		{
			if(timer.isActive()) 
				timer.stop();
			else
				timer.start();
			break;
		}
		case Qt::Key_Up:
		{
			int interval=timer.interval();
			interval=interval*2;

			if(interval<MIN_INTERVAL) interval=MIN_INTERVAL;
			if(interval>MAX_INTERVAL) interval=MAX_INTERVAL;

			timer.setInterval(interval);
			update();
			break;
		}
		case Qt::Key_Down:
		{
			int interval=timer.interval();
			interval=interval/2;

			if(interval<MIN_INTERVAL) interval=MIN_INTERVAL;
			if(interval>MAX_INTERVAL) interval=MAX_INTERVAL;

			timer.setInterval(interval);
			update();
			break;
		}
	}
}

void MainWindow::paintEvent(QPaintEvent*)
{
	if(world)
	{
		QPainter painter(this);
		for(int i=0;i<world->mapWidth();i++)
			for(int j=0;j<world->mapHeight();j++)
			{
				//first, draw the floor
				painter.drawPixmap(i*CELL_WIDTH,j*CELL_HEIGHT,
								   spriteList.value(FLOOR_SPRITE));
				
				Sprite sprite=world->sprite(i,j);
				painter.drawPixmap(i*CELL_WIDTH,j*CELL_HEIGHT,
								   spriteList.value(sprite));
			}
	}
	setWindowTitle(QString("Cycle: %1; interval: %2 ").arg(cycleCount).
				   arg(timer.interval()));
}
