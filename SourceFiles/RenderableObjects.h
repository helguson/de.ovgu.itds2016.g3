#pragma once
#include <string>
#include <QColor>
#include "Point3d.h"

class RenderableObjects
{
public:
	RenderableObjects();
	virtual ~RenderableObjects();

	void setVisibility(bool visibility);
	bool getVisibility();
	void setName(std::string name);
	std::string getName();
	Point3d getCenter();
	double getRadius();

	virtual void _computeCenter() = 0;
	virtual void _computeRadius() = 0;

	bool _isVisible;
	Point3d _center;
	double _radius;
	std::string _name;
};

