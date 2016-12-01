#include "Controller.h"


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
	view.setOnRequestLoadFile( [&model, &view](std::string path)->void {

		PointCloud3d pointCloud;
		FileLoader fileLoader;
		pointCloud.setPointsTo(fileLoader.load(path));
		model.setPointCloudTo(pointCloud);

		double fieldOfViewAngleOnYAxis = 45;
		double pointCloudRadius = pointCloud.getRadius();
		double overviewDistance = pointCloudRadius / tan((3.1415 / 180.0) * (fieldOfViewAngleOnYAxis / 2));

		model.setFieldOfViewAngleInYDirectionTo(fieldOfViewAngleOnYAxis);
		model.setNearClippingPlaneZTo(0.001);
		model.setFarClippingPlaneZTo(overviewDistance + 3 * pointCloudRadius);

		Point3d cameraPosition = pointCloud.getCenter() + Point3d(0, 0, overviewDistance);
		model.setWorldPositionTo(cameraPosition);

		model.setRotationAngleAroundYAxis(0);
		view.render(model.getPointCloud(), model.getModelProperties());
	});
	
	view.setOnRequestPaintGL( 
		[&view, &model]()->void { 
			view.render(model.getPointCloud(), model.getModelProperties());
		}
	);

	/*std::function<void(double, double)> onScroll = [&model](double offsetX, double offsetY)->void {

		double maxOffsetAccordingToObservations = 16;
		double dMax = 0.1;
		double factor = 1 + dMax*(offsetY / maxOffsetAccordingToObservations);

		double oldFieldOfViewAngle = model.getProjectionModel().getFieldOfViewAngleInYDirection();
		double newFieldOfViewAngle = oldFieldOfViewAngle * factor;

		model.getProjectionModel().setFieldOfViewAngleInYDirectionTo(newFieldOfViewAngle);
	};
	this->_view.setOnScrollCallbackTo(onScroll);*/

	//std::function<void(GLFWwindow*, int, int, int, int)> onKey = [&model](GLFWwindow* windowPtr, int key, int scancode, int action, int mods)->void {

	//	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
	//		
	//		if (key == GLFW_KEY_LEFT) {
	//			double oldAngle = model.getRotationAngleAroundYAxis();
	//			model.setRotationAngleAroundYAxis(oldAngle + 1);
	//		}

	//		if (key == GLFW_KEY_RIGHT) {
	//			double oldAngle = model.getRotationAngleAroundYAxis();
	//			model.setRotationAngleAroundYAxis(oldAngle - 1);
	//		}
	//	}
	//};
	//this->_view.setOnKeyCallbackTo(onKey);
}

Controller::~Controller() {

}

int Controller::startApplication() {
	return this->_view.startApplication();
}
