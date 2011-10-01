#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include <QTimer>
#include "../world/world.h"

class CreatureProfile;

class MainWindow : public QWidget
{
	Q_OBJECT

	QTimer timer;
	int cycleCount;
	QList<QPixmap> spriteList;
	
	World *world;
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
	void nextCycle();
protected:
	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
};

#endif // MAINWINDOW_H
