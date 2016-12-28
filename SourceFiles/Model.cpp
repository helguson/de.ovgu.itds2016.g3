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
	this->_renderableObjects.push_back((std::static_pointer_cast<RenderableObjects>(pointCloudPtr)));
}

void Model::addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet) {
	this->_pointDataSets.push_back(pointDataSet);
}

size_t Model::getNumberOfRenderableObjects() const {
	return this->_renderableObjects.size();
}

RenderableObjects& Model::getRenderableObjectAt(int index) {
	return *(this->_renderableObjects[index]);
}
std::vector<std::shared_ptr<RenderableObjects>>& Model::getVisibleObjects(std::vector<std::string> names)
{
	std::vector<std::shared_ptr<RenderableObjects>> visibleObjects = std::vector<std::shared_ptr<RenderableObjects>>(names.size());
	for(int nameIndex = 0; nameIndex < names.size(); nameIndex++)
	{
		for(int objIndex = 0; objIndex < this->_renderableObjects.size(); objIndex++)
		{
			if (this->_renderableObjects.at(objIndex)->getName() == names.at(nameIndex))
				visibleObjects.at(nameIndex) = this->_renderableObjects.at(objIndex);
		}
	}
	return visibleObjects;
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

void Model::smoothVisibleCloud(int index, double smoothFactor) {
		this->add(this->_pointClouds.at(index).get()->computeSmoothedVersionWith(smoothFactor));
}

void Model::thinVisibleCloud(int index, double thinningRadius) {
		this->add(this->_pointClouds.at(index).get()->computeThinnedVersionWith(thinningRadius));
}
