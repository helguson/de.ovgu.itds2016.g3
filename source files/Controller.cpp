#include "Controller.h"

#include <functional>
#include <iostream>

Controller::Controller()
	:
	_view(),
	_model(),
	_fileLoader()
{
	PointCloud3d pointCloud;

	pointCloud.setPointsTo(this->_fileLoader.load("data/cone.xyz"));

	this->_model.setPointCloudTo(pointCloud);
}

Controller::~Controller() {

}

void Controller::startMainLoop() {

	Model& model = this->_model;

	model.setShouldContinueMainLoopTo(true);

	std::function<void()> onDemandClosingOfWindow = [this]()->void {
		this->stopMainLoop();
	};

	this->_view.setOnDemandClosingOfWindowCallbackTo(onDemandClosingOfWindow);


	while (this->shouldContinueLooping()) {

		this->_view.render(this->_model.getPointCloud());
	}
}

void Controller::stopMainLoop() {

	this->_model.setShouldContinueMainLoopTo(false);
}

bool Controller::shouldContinueLooping() {

	return this->_model.getShouldContiniueMainLoop();
}