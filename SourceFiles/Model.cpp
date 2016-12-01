#include "Model.h"



Model::Model()
	:
	_pointDataSets(),
	_pointClouds(),
	_cameraModel(),
	_projectionModel(),
	_rotationAngleAroundYAxis(0)
{
}

Model::~Model()
{
}

void Model::add(std::shared_ptr<PointCloud3d> pointCloudPtr) {
	this->_pointClouds.push_back(pointCloudPtr);
}

void Model::addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet) {
	this->_pointDataSets.push_back(pointDataSet);
}

size_t Model::getNumberOfPointClouds() const {
	return this->_pointClouds.size();
}

PointCloud3d& Model::getPointCloudAt(int index) {
	return *(this->_pointClouds[index]);
}

CameraModel& Model::getCameraModel() {

	return this->_cameraModel;
}

ProjectionModel& Model::getProjectionModel() {

	return this->_projectionModel;
}

ModelProperties Model::getModelProperties() {
	return ModelProperties(
		this->_projectionModel.getFieldOfViewAngleInYDirection(),
		this->_projectionModel.getFarClippingPlaneZ(),
		this->_projectionModel.getNearClippingPlaneZ(),
		this->_rotationAngleAroundYAxis,
		this->_cameraModel.getWorldPosition());
}

void Model::setRotationAngleAroundYAxis(double angle) {
	this->_rotationAngleAroundYAxis = angle;
}

void Model::setWorldPositionTo(Point3d position)
{
	this->_cameraModel.setWorldPositionTo(position);
}

void Model::setFieldOfViewAngleInYDirectionTo(double angle)
{
	this->_projectionModel.setFieldOfViewAngleInYDirectionTo(angle);
}

void Model::setNearClippingPlaneZTo(double z)
{
	this->_projectionModel.setNearClippingPlaneZTo(z);
}

void Model::setFarClippingPlaneZTo(double z)
{
	this->_projectionModel.setFarClippingPlaneZTo(z);
}
