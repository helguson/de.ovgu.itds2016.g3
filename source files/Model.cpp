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

PointCloud3d& Model::getPointCloud(int index) {

	return this->_pointClouds[index];
}

PointCloud3d& Model::getSmoothedCloud(int index, double degree)
{
	PointCloud3d newCloud = this->_pointClouds[index].smooth(degree);
	this->_pointClouds.push_back(newCloud);
	this->_pointClouds.back().computeTree();
	return this->_pointClouds.back();
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

void Model::_recomputeTrees()
{
	for each (PointCloud3d cloud in this->_pointClouds)
	{
		cloud.computeTree();
	}
}
