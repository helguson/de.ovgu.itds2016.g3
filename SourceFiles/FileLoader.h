#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Point3d.h"


class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	std::shared_ptr<std::vector<Point3d>> load(std::string fileName);

};