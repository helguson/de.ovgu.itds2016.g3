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
	size_t getNumberOfRenderableObjects() const;
	std::shared_ptr<RenderableObjects> getRenderableObjectAt(int index);
	ModelProperties Model::getModelProperties();
	//change CameraModel
	void setWorldPositionTo(Point3d position);
	//change ProjectionModel
	void setFieldOfViewAngleInYDirectionTo(double angle);
	void setNearClippingPlaneZTo(double z);
	void setFarClippingPlaneZTo(double z);
	void setSceneCenterTo(Point3d position);

	bool smoothVisibleCloud(int index, double smoothFactor);
	bool thinVisibleCloud(int index, double smoothFactor);
	void addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet);

private:
	std::vector<std::shared_ptr<std::vector<Point3d>>> _pointDataSets;
	std::vector<std::shared_ptr<RenderableObjects>> _renderableObjects;
	CameraModel _cameraModel;
	ProjectionModel _projectionModel;
};

