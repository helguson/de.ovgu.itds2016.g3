#pragma once
class SettingsContainer
{
public:
	SettingsContainer();
	~SettingsContainer();

	bool showQuery;
	bool smooth;
	bool thinning;
	double smoothFactor;
	double nnRadius;
};

