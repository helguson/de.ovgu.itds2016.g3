#include "Model.h"



Model::Model()
	:
	_pointCloud(),
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

PointCloud3d& Model::getPointCloud(int index) {

	return this->_pointClouds[index];
}

PointCloud3d& Model::getSmoothedCloud(int index)
{
	return PointCloud3d();
}

CameraModel& Model::getCameraModel() {

	return this->_cameraModel;
}

ProjectionModel& Model::getProjectionModel() {

	return this->_projectionModel;
}

double Model::getRotationAngleAroundYAxis() {
	return this->_rotationAngleAroundYAxis;
}
void Model::setRotationAngleAroundYAxis(double angle) {
	this->_rotationAngleAroundYAxis = angle;
}
