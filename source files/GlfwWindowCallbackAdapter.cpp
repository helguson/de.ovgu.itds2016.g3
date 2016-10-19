#include "GlfwWindowCallbackAdapter.h"

void onDemandClosingCallbackForGlfw(GLFWwindow* windowPtr) {

	GlfwWindowCallbackAdapter* adapterPtr = (GlfwWindowCallbackAdapter*)glfwGetWindowUserPointer(windowPtr);

	adapterPtr->triggerOnDemandClosingWindowCallback();
}

GlfwWindowCallbackAdapter::GlfwWindowCallbackAdapter()
	:_onDemandClosingWindowCallback(nullptr)
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

void GlfwWindowCallbackAdapter::setOnDemandClosingWindowCallbackTo(std::function<void()> callback) {
	
	this->_onDemandClosingWindowCallback = callback;
}

void GlfwWindowCallbackAdapter::triggerOnDemandClosingWindowCallback() {

	if (this->_onDemandClosingWindowCallback != nullptr) {
		this->_onDemandClosingWindowCallback();
	}
}