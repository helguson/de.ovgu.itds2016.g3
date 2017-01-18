#pragma once
#include <QColor>
class SettingsContainer
{
public:
	SettingsContainer();
	SettingsContainer(bool sQ, bool sdt, bool ssh, QColor nn, QColor pc, QColor sc, QColor tc );
	~SettingsContainer();

	bool showQuery;
	bool showDistanceToPlane;
	bool showShading;
	double normalFactor;
	double smoothFactor;
	double thinRadius;
	QColor NNColor;
	QColor PCColor;
	QColor SCColor;
	QColor TCColor;
};

