#include "FileLoader.h"
#include <iostream>
#include <string>


//Here is the implementation of our file reader
void loadFileXYZ(const char* filename, std::vector<Point3d>& points)
{
	FILE* file = 0;
	int error = fopen_s(&file, filename, "rt"); //r= read, t=text
	if (error != 0)
	{
		std::cout << "file " << filename << " could not be opened!" << std::endl;
		return; //nothing can be done else -> end function
	}

	std::cout << "reading file: " << filename << std::endl;

	while (!feof(file)) //as long we have not reached the end-of-file
	{
		Point3d point;
		int items = fscanf_s(file, "%lf %lf %lf\n", &point.x, &point.y, &point.z);

		if (items != 3) //we ecpected that 3 values have been read (except we are already at the end of file)
		{
			std::cout << "file format error" << std::endl;
			break; //abort while loop
		}
		else
		{
			points.push_back(point); //add the current point to our point vector
		}
	}

	//dont forget to close to file
	fclose(file);

	unsigned int numberOfPoints = points.size();

	std::cout << "reading finished: " << numberOfPoints << " points have be read" << std::endl;
}


FileLoader::FileLoader() {

}

FileLoader::~FileLoader() {

}

std::shared_ptr<std::vector<Point3d>> FileLoader::load(std::string fileName) {	

		std::shared_ptr<std::vector<Point3d>> result = std::make_shared<std::vector<Point3d>>();
		loadFileXYZ(fileName.c_str(), *result);

	return result;
}

