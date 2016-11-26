#pragma once

#include "PointCloud3d.h"
#include "CameraModel.h"
#include "ProjectionModel.h"
#include "ThreeDTree.h"


class Model
{
public:
	Model();
	~Model();

	// TODO: replace with shared pointers or something else without copying all the points
	void setPointCloudTo(PointCloud3d& pointCloud);
	PointCloud3d& getPointCloud(int index);
	PointCloud3d& getSmoothedCloud(int index, double degree);
	CameraModel& getCameraModel();
	ProjectionModel& getProjectionModel();
	double getRotationAngleAroundYAxis();
	void setRotationAngleAroundYAxis(double angle);

private:
	std::vector<PointCloud3d> _pointClouds;
	CameraModel _cameraModel;
	ProjectionModel _projectionModel;
	double _rotationAngleAroundYAxis;
	void _recomputeTrees();
};

