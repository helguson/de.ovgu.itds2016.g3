#pragma once

#include "PointCloud3d.h"
#include "CameraModel.h"
#include "ProjectionModel.h"
#include "ThreeDTree.h"
#include "ModelProperties.h"
#include <memory>
#include <vector>

class Model
{
public:
	Model();
	~Model();

	void add(std::shared_ptr<PointCloud3d> pointCloudPtr);
	size_t getNumberOfPointClouds() const;
	PointCloud3d & getPointCloudAt(int index);
	std::vector<std::shared_ptr<PointCloud3d>>& getVisibleClouds();
	CameraModel& getCameraModel();
	ProjectionModel& getProjectionModel();
	void setRotationAngleAroundYAxis(double angle);
	ModelProperties Model::getModelProperties();
	//change CameraModel
	void setWorldPositionTo(Point3d position);
	//change ProjectionModel
	void setFieldOfViewAngleInYDirectionTo(double angle);
	void setNearClippingPlaneZTo(double z);
	void setFarClippingPlaneZTo(double z);

	void addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet);

private:
	std::vector<std::shared_ptr<std::vector<Point3d>>> _pointDataSets;
	std::vector<std::shared_ptr<PointCloud3d>> _pointClouds;
	std::vector<std::shared_ptr<PointCloud3d>> _visibleClouds;
	CameraModel _cameraModel;
	ProjectionModel _projectionModel;
	double _rotationAngleAroundYAxis;
};

