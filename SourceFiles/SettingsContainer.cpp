#include "SettingsContainer.h"



SettingsContainer::SettingsContainer()
	:
	showQuery(false),
	showDistanceToPlane(false),
	showShading(false),
	smoothFactor(0.0),
	normalFactor(0.0),
	thinRadius(0.0),
	NNColor(255,255,255),
	PCColor(255,255,255),
	SCColor(255,255,255),
	TCColor(255,255,255)
{
}

SettingsContainer::SettingsContainer(bool sQ, bool sdt, bool ssh, QColor nn, QColor pc, QColor sc, QColor tc )
	:
	showQuery(sQ),
	showDistanceToPlane(sdt),
	showShading(ssh),
	smoothFactor(0.0),
	thinRadius(0.0),
	normalFactor(0.0),
	NNColor(nn),
	PCColor(pc),
	SCColor(sc),
	TCColor(tc)
{
}


SettingsContainer::~SettingsContainer()
{
}
