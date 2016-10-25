#pragma once

#include "PointCloud3d.h"


class Model
{
public:
	Model();
	~Model();

	// TODO: replace with shared pointers or something else without copying all the points
	void setPointCloudTo(PointCloud3d pointCloud);
	PointCloud3d getPointCloud();

private:
	PointCloud3d _pointCloud;
};

