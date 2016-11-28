#include "Model.h"

Model::Model()
	:
	_pointDataSets(),
	_pointClouds(),
	_cameraModel(),
	_projectionModel(),
	_rotationAngleAroundYAxis(0)
{}

Model::~Model()
{}

void Model::add(std::shared_ptr<PointCloud3d> pointCloudPtr) {
	this->_pointClouds.push_back(pointCloudPtr);
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

double Model::getRotationAngleAroundYAxis() const {
	return this->_rotationAngleAroundYAxis;
}

void Model::setRotationAngleAroundYAxis(double angle) {
	this->_rotationAngleAroundYAxis = angle;
}

void Model::addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet) {
	this->_pointDataSets.push_back(pointDataSet);
}