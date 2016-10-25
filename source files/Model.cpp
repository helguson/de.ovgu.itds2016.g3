#include "Model.h"



Model::Model()
	:
	_pointCloud(),
	_shouldContinueMainLoop(false)
{
}

Model::~Model()
{
}

void Model::setPointCloudTo(PointCloud3d pointCloud) {

	this->_pointCloud = pointCloud;
}

PointCloud3d Model::getPointCloud() {

	return this->_pointCloud;
}

void Model::setShouldContinueMainLoopTo(bool value) {

	this->_shouldContinueMainLoop = value;
}

bool Model::getShouldContiniueMainLoop() {
	
	return this->_shouldContinueMainLoop;
}
