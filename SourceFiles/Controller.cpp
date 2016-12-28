#include "Controller.h"

#include <functional>
#include <iostream>
#include "util.h"

Controller::Controller(int numberOfArguments, char** arguments)
	:
	_view(numberOfArguments, arguments),
	_model()
{
	QtView& view = this->_view;
	Model& model = this->_model;
	//################################
	//### setup ui event callbacks ###
	//################################

	//function if new File loaded
	view.setOnRequestLoadFile( [this, &model, &view](std::string path)->void {

	std::shared_ptr<std::vector<Point3d>> pointData = FileLoader().load(path);
	model.addPointDataSet(pointData);
	
	std::shared_ptr<PointCloud3d> pointCloudPtr = std::make_shared<PointCloud3d>(
		this->_getStoreCreatedDataFunctor(),
		*util::computePointPtrVectorFrom(*pointData)
	);
	model.add(pointCloudPtr);
	double fieldOfViewAngleOnYAxis = 45; 
	double pointCloudRadius = pointCloudPtr->getRadius();
	double overviewDistance = pointCloudRadius / tan((3.1415 / 180.0) * (fieldOfViewAngleOnYAxis / 2));

	ProjectionModel& projectionModel = model.getProjectionModel();
	projectionModel.setFieldOfViewAngleInYDirectionTo(fieldOfViewAngleOnYAxis);
	projectionModel.setNearClippingPlaneZTo(0.001);
	projectionModel.setFarClippingPlaneZTo(overviewDistance + 3*pointCloudRadius);

	Point3d cameraPosition = pointCloudPtr->getCenter() + Point3d(0, 0, overviewDistance);
	model.getCameraModel().setWorldPositionTo(cameraPosition);

	model.setRotationAngleAroundYAxis(0);

	model.setRotationAngleAroundYAxis(0);
	view.render(model.getVisibleObjects(view.getSelectedNames()), model.getModelProperties());
	});
	
	view.setOnRequestPaintGL( 
		[&view, &model]()->void { 
			if (model.getNumberOfPointClouds() > 0) {
				view.render(model.getVisibleClouds(), model.getModelProperties());
			}
		}
	);

	view.setOnRequestSmoothCloud(
		[&view, &model]()->void {
		if (model.getNumberOfPointClouds() == 0) return;
		model.smoothVisibleClouds(view.getSettings().smoothFactor);
		view.render(model.getVisibleClouds(), model.getModelProperties());
	}
	);

	view.setOnRequestThinCloud(
		[&view, &model]()->void {
		if (model.getNumberOfPointClouds() == 0) return;
		view.render(model.getVisibleClouds(), model.getModelProperties());
	}
	);
}

Controller::~Controller() {

}

int Controller::startApplication(){
	return this->_view.startApplication();
}

std::function<void(std::shared_ptr<std::vector<Point3d>>)> Controller::_getStoreCreatedDataFunctor() {

	Model & model = this->_model;
	return [&model](std::shared_ptr<std::vector<Point3d>> pointData)->void {
		model.addPointDataSet(pointData);
	};
}
