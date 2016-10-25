#pragma once

#include <string>
#include <vector>

#include "Point3d.h"


class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	std::vector<Point3d> load(std::string);
};