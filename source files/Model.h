#pragma once

#include "PointCloud3d.h"
#include "CameraModel.h"
#include "ProjectionModel.h"


class Model
{
public:
	Model();
	~Model();

	// TODO: replace with shared pointers or something else without copying all the points
	void setPointCloudTo(PointCloud3d pointCloud);
	PointCloud3d getPointCloud();
	CameraModel& getCameraModel();
	ProjectionModel& getProjectionModel();

private:
	PointCloud3d _pointCloud;
	CameraModel _cameraModel;
	ProjectionModel _projectionModel;
};

