#include "Controller.h"


Controller::Controller(int argc, char* argv[])
	:
	_view(),
	_model()
{
	//################################
	//### setup ui event callbacks ###
	//################################
	Model& model = this->_model;

	//function if new File loaded
	std::function<void(std::string)> onNewFile = [this](std::string path)->void {

		PointCloud3d pointCloud;
		FileLoader fileLoader;
		pointCloud.setPointsTo(fileLoader.load(path));
		this->_model.setPointCloudTo(pointCloud);

		double fieldOfViewAngleOnYAxis = 45;
		double pointCloudRadius = pointCloud.getRadius();
		double overviewDistance = pointCloudRadius / tan((3.1415 / 180.0) * (fieldOfViewAngleOnYAxis / 2));

		this->_model.setFieldOfViewAngleInYDirectionTo(fieldOfViewAngleOnYAxis);
		this->_model.setNearClippingPlaneZTo(0.001);
		this->_model.setFarClippingPlaneZTo(overviewDistance + 3 * pointCloudRadius);

		Point3d cameraPosition = pointCloud.getCenter() + Point3d(0, 0, overviewDistance);
		this->_model.setWorldPositionTo(cameraPosition);

		this->_model.setRotationAngleAroundYAxis(0);
		renderData();
	};
	this->_view.setOnNewFileLoaded(onNewFile);

	std::function<void()> onPaintRequest = [this]()->void {
		this->_view.renderData(this->_model.getPointCloud(), this->_model.getModelProperties());
	};
	this->_view.setOnPaintRequest(onPaintRequest);

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

	this->_view.show();
}

Controller::~Controller() {

}

void Controller::renderData()
{		
	this->_view.renderData( this->_model.getPointCloud(),  this->_model.getModelProperties());
}

