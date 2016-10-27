#include "Controller.h"

#include <functional>
#include <iostream>

Controller::Controller()
	:
	_view(),
	_model(),
	_fileLoader(),
	_shouldContiniueLooping(false)
{
	PointCloud3d pointCloud;
	pointCloud.setPointsTo(this->_fileLoader.load("data/cone.xyz"));
	this->_model.setPointCloudTo(pointCloud);

	double fieldOfViewAngleOnYAxis = 45;
	const double overviewDistance = pointCloud.getRadius() / tan((3.1415 / 180.0) * (fieldOfViewAngleOnYAxis / 2));
	Point3d cameraPosition = pointCloud.getCenter() + Point3d(0, 0, overviewDistance);
	this->_model.getCameraModel().setWorldPositionTo(cameraPosition);
}

Controller::~Controller() {

}

void Controller::startMainLoop() {

	Model& model = this->_model;

	std::function<void()> onDemandClosingOfWindow = [this]()->void {
		this->stopMainLoop();
	};

	this->_view.setOnDemandClosingOfWindowCallbackTo(onDemandClosingOfWindow);

	this->_shouldContiniueLooping = true;

	while (this->shouldDoNextLoop()) {

		this->_view.render(
			this->_model.getPointCloud(),
			this->_model.getCameraModel()
		);
	}
}

void Controller::stopMainLoop() {

	this->_shouldContiniueLooping = false;
}

bool Controller::shouldDoNextLoop() {

	return this->_shouldContiniueLooping;
}