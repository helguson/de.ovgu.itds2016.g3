#pragma once
#include <QColor>
class SettingsContainer
{
public:
	SettingsContainer();
	SettingsContainer(bool sQ, QColor nn, QColor pc, QColor sc, QColor tc );
	~SettingsContainer();

	bool showQuery;
	double smoothFactor;
	double thinRadius;
	QColor NNColor;
	QColor PCColor;
	QColor SCColor;
	QColor TCColor;
};

