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
	double pointCloudRadius = pointCloud.getRadius();
	double overviewDistance = pointCloudRadius / tan((3.1415 / 180.0) * (fieldOfViewAngleOnYAxis / 2));

	ProjectionModel& projectionModel = this->_model.getProjectionModel();
	projectionModel.setFieldOfViewAngleInYDirectionTo(fieldOfViewAngleOnYAxis);
	projectionModel.setNearClippingPlaneZTo(0.001);
	projectionModel.setFarClippingPlaneZTo(overviewDistance + 3*pointCloudRadius);

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

	std::function<void(double, double)> onScroll = [](double offsetX, double offsetY)->void {
		std::cout << "dx: " << offsetX << ", dy: " << offsetY << std::endl;
	};
	this->_view.setOnScrollCallbackTo(onScroll);


	this->_shouldContiniueLooping = true;

	while (this->shouldDoNextLoop()) {

		this->_view.render(
			this->_model.getPointCloud(),
			this->_model.getCameraModel(),
			this->_model.getProjectionModel()
		);
	}
}

void Controller::stopMainLoop() {

	this->_shouldContiniueLooping = false;
}

bool Controller::shouldDoNextLoop() {

	return this->_shouldContiniueLooping;
}