#include "Model.h"



Model::Model()
	:
	_pointCloud()
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
