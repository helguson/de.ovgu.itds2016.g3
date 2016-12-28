#pragma once
#include <string>
#include <QColor>

class RenderableObjects
{
public:
	RenderableObjects();
	~RenderableObjects();

	void setVisibility(bool visibility);
	bool getVisibility();
	void setName(std::string name);
	std::string getName();
private:
	bool _isVisible;
	std::string _name;
};

