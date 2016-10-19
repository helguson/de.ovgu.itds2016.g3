#pragma once

#include "GLFW\glfw3.h"

#include <functional>

/*
motivation:
 - the callback functions of glfw have to be c functions
 - this excludes c++ class methods like lambdas with capture and others
goal:
 - allow c++ class methods
method:
 - indirect callbacks of glfw on this adapter that allows c++ class methods
 - using http://www.glfw.org/docs/latest/window_guide.html#window_userptr
thanks to:
 - the members of the glfw irc for their explanations on the problem and for 
   the hint on this method
 */
class GlfwWindowCallbackAdapter
{
public:
	GlfwWindowCallbackAdapter();
	~GlfwWindowCallbackAdapter();

	void hookInto(GLFWwindow* windowPtr);
	void setOnDemandClosingWindowCallbackTo(std::function<void()> callback);

	void triggerOnDemandClosingWindowCallback();

private:
	std::function<void()> _onDemandClosingWindowCallback;
};
