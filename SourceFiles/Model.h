#pragma once

#include "PointCloud3d.h"
#include "CameraModel.h"
#include "ProjectionModel.h"
#include "ThreeDTree.h"
#include "ModelProperties.h"
#include "BestFitLine.h"
#include "BestFitPlane.h"
#include "BestFitSphere.h"
#include <memory>
#include <vector>
#include <qmatrix4x4.h>

class Model
{
public:
	Model();
	~Model();

	void add(std::shared_ptr<PointCloud3d> pointCloudPtr);
	void add(std::shared_ptr<BestFitLine> bfLinePtr);
	void add(std::shared_ptr<BestFitPlane> bfPlanePtr);
	void addNormals(std::shared_ptr<PointCloud3d> pointCloudPtr, double radius);
	void add(std::shared_ptr<BestFitSphere> bfSpherePtr);
	size_t getNumberOfRenderableObjects() const;
	std::shared_ptr<RenderableObjects> getRenderableObjectAt(int index);
	double getFieldOfViewAngleInYDirection();
	Point3d getSceneCenter();
	Point3d getCameraPosition();
	QMatrix4x4 getModelViewProjectionMatrix();
	QMatrix4x4 getModelViewMatrix();

	//change CameraModel
	void reinitializeCamera();
	void setCameraPositionTo(Point3d position);
	void setSceneCenterTo(Point3d position);
	void rotateCamera(Point3d axis, double angle);
	void translateCamera(Point3d axis);
	//change ProjectionModel
	void setFieldOfViewAngleInYDirectionTo(double angle);
	void setNearClippingPlaneZTo(double z);
	void setFarClippingPlaneZTo(double z);
	void setWindowSize(double width, double height);


	bool smoothVisibleCloud(int index, double smoothFactor);
	bool thinVisibleCloud(int index, double smoothFactor);
	bool bfLine(int selectedIndex);
	bool bfPlane(int selectedIndex);
	bool bfSphere(int selectedIndex);
	void addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet);

private:
	std::vector<std::shared_ptr<std::vector<Point3d>>> _pointDataSets;
	std::vector<std::shared_ptr<RenderableObjects>> _renderableObjects;
	std::vector<std::shared_ptr<std::vector<Point3d>>> _normalVectorSets;
	CameraModel _cameraModel;
	ProjectionModel _projectionModel;
	
	//TODO:
	QMatrix4x4 _modelMatrix;
};

