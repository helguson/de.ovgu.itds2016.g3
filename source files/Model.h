#pragma once

#include "PointCloud3d.h"
#include "CameraModel.h"
#include "ProjectionModel.h"
#include "ThreeDTree.h"
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

	CameraModel& getCameraModel();
	ProjectionModel& getProjectionModel();
	double getRotationAngleAroundYAxis() const;
	void setRotationAngleAroundYAxis(double angle);

	void addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet);

private:
	std::vector<std::shared_ptr<std::vector<Point3d>>> _pointDataSets;
	std::vector<std::shared_ptr<PointCloud3d>> _pointClouds;
	CameraModel _cameraModel;
	ProjectionModel _projectionModel;
	double _rotationAngleAroundYAxis;
};

