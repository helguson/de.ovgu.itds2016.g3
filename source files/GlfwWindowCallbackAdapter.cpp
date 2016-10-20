#include "GlfwWindowCallbackAdapter.h"

//############################
//### c functions for glfw ###
//############################
void onDemandClosingCallbackForGlfw(GLFWwindow* windowPtr) {

	GlfwWindowCallbackAdapter* adapterPtr = (GlfwWindowCallbackAdapter*)glfwGetWindowUserPointer(windowPtr);

	adapterPtr->triggerOnDemandClosingWindowCallback(windowPtr);
}

void onResizeCallbackForGlfw(GLFWwindow* windowPtr, int width, int height) {

	GlfwWindowCallbackAdapter* adapterPtr = (GlfwWindowCallbackAdapter*)glfwGetWindowUserPointer(windowPtr);

	adapterPtr->triggerOnResizeCallback(windowPtr, width, height);
}

void onMousePressCallbackForGlfw(GLFWwindow* windowPtr, int button, int action, int mods) {

	GlfwWindowCallbackAdapter* adapterPtr = (GlfwWindowCallbackAdapter*)glfwGetWindowUserPointer(windowPtr);

	adapterPtr->triggerOnMousePressCallback(windowPtr, button, action, mods);
}

void onMouseMoveCallbackForGlfw(GLFWwindow* windowPtr, double currentPositionX, double currentPositionY) {

	GlfwWindowCallbackAdapter* adapterPtr = (GlfwWindowCallbackAdapter*)glfwGetWindowUserPointer(windowPtr);

	adapterPtr->triggerOnMouseMoveCallback(windowPtr, currentPositionX, currentPositionY);
}

void onMouseWheelCallbackForGlfw(GLFWwindow* windowPtr, double offsetX, double offsetY) {

	GlfwWindowCallbackAdapter* adapterPtr = (GlfwWindowCallbackAdapter*)glfwGetWindowUserPointer(windowPtr);

	adapterPtr->triggerOnMouseWheelCallback(windowPtr, offsetX, offsetY);
}

//############################
//### class implementation ###
//############################
GlfwWindowCallbackAdapter::GlfwWindowCallbackAdapter()
	:
	_onDemandClosingWindowCallback(nullptr),
	_onResize(nullptr),
	_onMousePress(nullptr),
	_onMouseMove(nullptr),
	_onMouseWheel(nullptr)
{
}

GlfwWindowCallbackAdapter::~GlfwWindowCallbackAdapter() {
	// TODO?
}

void GlfwWindowCallbackAdapter::hookInto(GLFWwindow* windowPtr) {
	
	// hook adapter
	glfwSetWindowUserPointer(
		windowPtr,
		this
	);

	// hook callbacks that lead back to adapter
	glfwSetWindowCloseCallback(
		windowPtr,
		onDemandClosingCallbackForGlfw
	);
}

//##############
//### setter ###
//##############
void GlfwWindowCallbackAdapter::setOnDemandClosingWindowCallbackTo(std::function<void(GLFWwindow*)> callback) {
	
	this->_onDemandClosingWindowCallback = callback;
}

void GlfwWindowCallbackAdapter::setOnResizeCallbackTo(std::function<void(GLFWwindow*, int, int)> callback) {

	this->_onResize = callback;
}

void GlfwWindowCallbackAdapter::setOnMousePressCallbackTo(std::function<void(GLFWwindow*, int, int, int)> callback) {

	this->_onMousePress = callback;
}

void GlfwWindowCallbackAdapter::setOnMouseMoveCallbackTo(std::function<void(GLFWwindow*, double, double)> callback) {

	this->_onMouseMove = callback;
}

void GlfwWindowCallbackAdapter::setOnMouseWheelCallbackTo(std::function<void(GLFWwindow*, double, double)> callback) {

	this->_onMouseWheel = callback;
}

//###############
//### trigger ###
//###############
void GlfwWindowCallbackAdapter::triggerOnDemandClosingWindowCallback(GLFWwindow* windowPtr) {

	if (this->_onDemandClosingWindowCallback != nullptr) {
		this->_onDemandClosingWindowCallback(windowPtr);
	}
}

void GlfwWindowCallbackAdapter::triggerOnResizeCallback(GLFWwindow* windowPtr, int width, int height) {

	if (this->_onResize != nullptr) {
		this->_onResize(windowPtr, width, height);
	}
}

void GlfwWindowCallbackAdapter::triggerOnMousePressCallback(GLFWwindow* windowPtr, int button, int action, int mods) {

	if (this->_onMousePress != nullptr) {
		this->_onMousePress(windowPtr, button, action, mods);
	}
}

void GlfwWindowCallbackAdapter::triggerOnMouseMoveCallback(GLFWwindow* windowPtr, double currentPositionX, double currentPositionY) {

	if (this->_onMouseMove != nullptr) {
		this->_onMouseMove(windowPtr, currentPositionX, currentPositionY);
	}
}

void GlfwWindowCallbackAdapter::triggerOnMouseWheelCallback(GLFWwindow* windowPtr, double offsetX, double offsetY) {

	if (this->_onMouseWheel != nullptr) {
		this->_onMouseWheel(windowPtr, offsetX, offsetY);
	}
}