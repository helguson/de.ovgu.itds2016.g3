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
	void setOnDemandClosingWindowCallbackTo(std::function<void(GLFWwindow*)> callback);
	void setOnResizeCallbackTo(std::function<void(GLFWwindow*, int, int)> callback);
	void setOnMousePressCallbackTo(std::function<void(GLFWwindow*, int, int, int)> callback);
	void setOnMouseMoveCallbackTo(std::function<void(GLFWwindow*, double, double)> callback);
	void setOnMouseWheelCallbackTo(std::function<void(GLFWwindow*, double, double)> callback);
	void setOnKeyCallbackTo(std::function<void(GLFWwindow*, int, int, int, int)> callback);

	void triggerOnDemandClosingWindowCallback(GLFWwindow* windowPtr);
	void triggerOnResizeCallback(GLFWwindow* windowPtr, int width, int height);
	void triggerOnMousePressCallback(GLFWwindow* windowPtr, int button, int action, int mods);
	void triggerOnMouseMoveCallback(GLFWwindow* windowPtr, double currentPositionX, double currentPositionY);
	void triggerOnMouseWheelCallback(GLFWwindow* windowPtr, double offsetX, double offsetY);
	void triggerOnKeyCallback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods);

private:
	std::function<void(GLFWwindow*)> _onDemandClosingWindowCallback;
	std::function<void(GLFWwindow*, int, int)> _onResize;
	std::function<void(GLFWwindow*, int, int, int)> _onMousePress;
	std::function<void(GLFWwindow*, double, double)> _onMouseMove;
	std::function<void(GLFWwindow*, double, double)> _onMouseWheel;
	std::function<void(GLFWwindow*, int, int, int, int)> _onKey;
};
