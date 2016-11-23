#include "CameraModel.h"

CameraModel::CameraModel()
	:
	_worldPosition()
{
}

CameraModel::~CameraModel() {

}

void CameraModel::setWorldPositionTo(Point3d position) {

	this->_worldPosition = position;
}

Point3d CameraModel::getWorldPosition() {
	
	return this->_worldPosition;
}