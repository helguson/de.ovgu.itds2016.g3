#include "RenderableObjects.h"



RenderableObjects::RenderableObjects() :
	_isVisible(true),
	_center(Point3d()),
	_radius(0.0),
	_name("")
{}

RenderableObjects::~RenderableObjects()
{}

void RenderableObjects::setVisibility(bool visibility)
{
	_isVisible = visibility;
}

void RenderableObjects::setName(std::string name)
{
	this->_name = name;
}

bool RenderableObjects::getVisibility()
{
	return _isVisible;
}

std::string RenderableObjects::getName()
{
	return this->_name;
}

Point3d RenderableObjects::getCenter() const
{
	return this->_center;
}

double RenderableObjects::getRadius() const
{
	return this->_radius;
}
