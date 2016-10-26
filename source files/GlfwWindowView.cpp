#include "GlfwWindowView.h"
#include <iostream>

GlfwWindowView::GlfwWindowView()
	:
	_isGlfwInitialized(false),
	_isWindowInitialized(false),
	_windowPtr(nullptr),
	_callbackAdapter()
{

	this->_initializeLibrary();
	this->_initializeGlfwWindow();
	this->_initializeCallbackAdapter();
}

GlfwWindowView::~GlfwWindowView() {

	glfwTerminate();
	// TODO: free stuff
}

bool GlfwWindowView::isCorrectlyInitialized() {

	return this->_isGlfwInitialized && this->_isWindowInitialized;
}

void GlfwWindowView::_initializeLibrary() {

	this->_isGlfwInitialized = glfwInit() == GLFW_TRUE;
}

void GlfwWindowView::_initializeGlfwWindow() {

	this->_windowPtr = glfwCreateWindow(
		640,
		480,
		"hello world",
		NULL,
		NULL
	);

	this->_isWindowInitialized = this->_windowPtr != nullptr;
}

void GlfwWindowView::_initializeCallbackAdapter() {

	if (this->_isWindowInitialized) {
		this->_callbackAdapter.hookInto(this->_windowPtr);
	}
}

void GlfwWindowView::render(PointCloud3d pointCloud) {

	if (this->isCorrectlyInitialized()) {
		this->_initializeImage();
		this->_renderImage(pointCloud);
		this->_showImage();
		this->_pollInteractionsWithWindow();
	}
	else {
		// TODO: log warning
	}
}

void GlfwWindowView::_initializeImage() {

	// Make the window's context current
	glfwMakeContextCurrent(this->_windowPtr);

	// clear image buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
}

void GlfwWindowView::_renderImage(PointCloud3d pointCloud) {

	// TODO
	glViewport(0, 0, this->_getWidth(), this->_getHeight());
	_setPerspective(pointCloud.getRadius());
	_setCameraTransformation(pointCloud.getCenter(),pointCloud.getRadius());


	// render point cloud
	glPointSize(2);

	if (!pointCloud.getPoints().empty())
	{ /* Drawing Points with VertexArrays */
		glColor3ub(255, 255, 255);
		glEnableClientState(GL_VERTEX_ARRAY); //enable data upload to GPU
		glVertexPointer(3, GL_DOUBLE, sizeof(Point3d), &pointCloud.getPoints()[0]);

		//draw point cloud
		glDrawArrays(GL_POINTS, 0, (unsigned int)pointCloud.getPoints().size());
		glDisableClientState(GL_VERTEX_ARRAY);  //disable data upload to GPU
	}
}

void GlfwWindowView::_showImage() {

	// Swap front and back buffers
	glfwSwapBuffers(this->_windowPtr);
}

void GlfwWindowView::_pollInteractionsWithWindow() {

	// Poll for and process events
	glfwPollEvents();
}

void GlfwWindowView::setOnDemandClosingOfWindowCallbackTo(std::function<void()> callback) {

	this->_callbackAdapter.setOnDemandClosingWindowCallbackTo(
		[callback](GLFWwindow* windowPtr)->void { callback();  }
	);
}

void GlfwWindowView::_setPerspective(double radius) {
	int windowHeight, windowWidth;
	glfwGetFramebufferSize(this->_windowPtr, &windowWidth, &windowHeight);
	const double D = radius / sin((3.1415 / 180.0) * (45.0 / 2));

	const double zFar = D + radius;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(double)windowWidth/(double)windowHeight,0.001,zFar);
}

void GlfwWindowView::_setCameraTransformation(Point3d pointCloudCenter, double radius)
{
	Point3d upVector = Point3d(0, 1, 0);
	const double D = radius / tan((3.1415 / 180.0) * (45.0 / 2));
	Point3d cameraCenter = pointCloudCenter + Point3d(0,D,0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		cameraCenter.x, cameraCenter.y, cameraCenter.z, 
		pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z, 
		upVector.x, upVector.y, upVector.z
	);
}

int GlfwWindowView::_getWidth() {
	int width, height;
	glfwGetFramebufferSize(this->_windowPtr,&width,&height);
	return width;
}

int GlfwWindowView::_getHeight() {
	int width, height;
	glfwGetFramebufferSize(this->_windowPtr, &width, &height);
	return height;
}