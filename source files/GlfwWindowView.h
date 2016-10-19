#pragma once

#include <GLFW\glfw3.h>
#include <functional>

#include "GlfwWindowCallbackAdapter.h"

class GlfwWindowView {

public:
	GlfwWindowView();
	~GlfwWindowView();

	void render();
	bool isCorrectlyInitialized();

	void setOnDemandClosingOfWindowCallbackTo(std::function<void()> callback);
private:
	bool _isGlfwInitialized;
	bool _isWindowInitialized;
	GLFWwindow* _windowPtr;
	GlfwWindowCallbackAdapter _callbackAdapter;

	void _initializeLibrary();
	void _initializeGlfwWindow();
	void _initializeCallbackAdapter();
	void _initializeImage();
	void _renderImage();
	void _showImage();
	void _pollInteractionsWithWindow();
};