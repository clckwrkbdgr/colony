TARGET = colony
VERSION = 0.9.0
TEMPLATE = app

MOC_DIR = tmp
OBJECTS_DIR = tmp

CONFIG += qt release

SOURCES += main.cpp \
	mainwindow.cpp \
	scenarioparser.cpp
HEADERS += mainwindow.h \
	scenarioparser.h

SOURCES += world/world.cpp \
	world/creature.cpp \
	world/plant.cpp \
	world/animal.cpp \
	world/cell.cpp \
	world/map.cpp \
	world/scenario.cpp
HEADERS += world/world.h \
	world/creature.h \
	world/plant.h \
	world/animal.h \
	world/cell.h \
	world/map.h \
	world/dumblist.h \
	world/scenario.h \
	world/worldtypes.h

