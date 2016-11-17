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

	this->_pointCloud = pointCloud;
}

PointCloud3d& Model::getPointCloud() {

	return this->_pointCloud;
}

PointCloud3d& Model::getSmoothedCloud()
{
	this->_pointCloud.smooth(0.5);
	return this->_pointCloud;
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
