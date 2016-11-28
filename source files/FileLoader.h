#pragma once

#include <string>
#include <vector>
#include <memory>
#include "OpenFileDialog.h"
#include "Point3d.h"


class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	std::shared_ptr<std::vector<Point3d>> load();
};