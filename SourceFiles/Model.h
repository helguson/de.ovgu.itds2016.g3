#pragma once

#include "PointCloud3d.h"
#include "CameraModel.h"
#include "ProjectionModel.h"
#include "ThreeDTree.h"
#include "ModelProperties.h"


class Model
{
public:
	Model();
	~Model();

	// TODO: replace with shared pointers or something else without copying all the points
	void setPointCloudTo(PointCloud3d& pointCloud);
	PointCloud3d& getPointCloud();
	PointCloud3d& getPointCloud(int index);
	void setRotationAngleAroundYAxis(double angle);
	ModelProperties Model::getModelProperties();
	//change CameraModel
	void setWorldPositionTo(Point3d position);
	//change ProjectionModel
	void setFieldOfViewAngleInYDirectionTo(double angle);
	void setNearClippingPlaneZTo(double z);
	void setFarClippingPlaneZTo(double z);

private:
	std::vector<PointCloud3d> _pointClouds;
	CameraModel _cameraModel;
	ProjectionModel _projectionModel;
	double _rotationAngleAroundYAxis;
	void _recomputeTrees();
};

