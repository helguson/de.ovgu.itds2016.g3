#include "RenderableObjects.h"



RenderableObjects::RenderableObjects()
{
}


RenderableObjects::~RenderableObjects()
{
}

void RenderableObjects::setVisibility(bool visibility)
{
	_isVisible = visibility;
}

bool RenderableObjects::getVisibility()
{
	return _isVisible;
}

void RenderableObjects::setName(std::string name)
{
	this->_name = name;
}

std::string RenderableObjects::getName()
{
	return this->_name;
}
