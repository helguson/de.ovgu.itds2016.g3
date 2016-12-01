#include "Model.h"



Model::Model()
	:
	_pointClouds(),
	_cameraModel(),
	_projectionModel(),
	_rotationAngleAroundYAxis(0)
{
}

Model::~Model()
{
}

void Model::setPointCloudTo(PointCloud3d& pointCloud) {

	this->_pointClouds.push_back(pointCloud);
	this->_pointClouds.back().computeTree();
}

PointCloud3d& Model::getPointCloud() {
	if(!this->_pointClouds.empty())
	return this->_pointClouds.back();
	else return PointCloud3d();
}

PointCloud3d& Model::getPointCloud(int index) {
	if (index >= 0 && index <= this->_pointClouds.size())
		return this->_pointClouds[index];
	else{ }
		//Todo
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

void Model::_recomputeTrees()
{
	for each (PointCloud3d cloud in this->_pointClouds)
	{
		cloud.computeTree();
	}
}
