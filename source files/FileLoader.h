#pragma once

#include <string>
#include <vector>
#include "OpenFileDialog.h"
#include "Point3d.h"


class FileLoader
{
public:
	FileLoader();
	~FileLoader();

	std::vector<Point3d> load();
	char * wchar_to_string(_TCHAR* widechar);
};