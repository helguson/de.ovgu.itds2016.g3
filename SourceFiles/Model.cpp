#include "Model.h"



Model::Model()
	:
	_pointDataSets(),
	_renderableObjects(),
	_cameraModel(),
	_projectionModel()
{
}

Model::~Model()
{
}

void Model::add(std::shared_ptr<PointCloud3d> pointCloudPtr) {
	this->_renderableObjects.push_back(pointCloudPtr);
}

void Model::addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet) {
	this->_pointDataSets.push_back(pointDataSet);
}

size_t Model::getNumberOfRenderableObjects() const {
	return this->_renderableObjects.size();
}

std::shared_ptr<RenderableObjects> Model::getRenderableObjectAt(int index) {
	return this->_renderableObjects.at(index);
}


ModelProperties Model::getModelProperties() {
	return ModelProperties(
		this->_projectionModel.getFieldOfViewAngleInYDirection(),
		this->_projectionModel.getFarClippingPlaneZ(),
		this->_projectionModel.getNearClippingPlaneZ(),
		this->_cameraModel.getWorldPosition(),
		this->_cameraModel.getSceneCenter());
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

void Model::setSceneCenterTo(Point3d position)
{
	this->_cameraModel.setSceneCenterTo(position);
}

bool Model::smoothVisibleCloud(int index, double smoothFactor) {
	std::shared_ptr<PointCloud3d> cloudPtr = std::dynamic_pointer_cast<PointCloud3d>(this->_renderableObjects.at(index));
	if (cloudPtr) {
		this->add(cloudPtr->computeSmoothedVersionWith(smoothFactor));
		return true;
	}
	return false;
}

bool Model::thinVisibleCloud(int index, double thinningRadius) {
	std::shared_ptr<PointCloud3d> cloudPtr = std::dynamic_pointer_cast<PointCloud3d>(this->_renderableObjects.at(index));
	if (cloudPtr) {
		this->add(cloudPtr->computeThinnedVersionWith(thinningRadius));
		return true;
	}
	return false;
}
