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
	glClear(GL_COLOR_BUFFER_BIT); // clear image buffer
}

void GlfwWindowView::_renderImage(PointCloud3d pointCloud) {

	// TODO
	// setTransformationsFor(camera)


	// render point cloud

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