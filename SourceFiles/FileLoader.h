#pragma once

#include <vector>
#include <string>
#include "Point3d.h"


class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	std::vector<Point3d> load(std::string fileName);
};