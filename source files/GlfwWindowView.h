#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <functional>

#include "PointCloud3d.h"
#include "CameraModel.h"
#include "ProjectionModel.h"

#include "GlfwWindowCallbackAdapter.h"

class GlfwWindowView {

public:
	GlfwWindowView();
	~GlfwWindowView();

	void render(PointCloud3d pointCloud, CameraModel cameraModel, ProjectionModel projectionModel);
	bool isCorrectlyInitialized();

	void setOnDemandClosingOfWindowCallbackTo(std::function<void()> callback);
	void setOnScrollCallbackTo(std::function<void(double, double)> callback);
	
private:
	bool _isGlfwInitialized;
	bool _isWindowInitialized;
	GLFWwindow* _windowPtr;
	GlfwWindowCallbackAdapter _callbackAdapter;
	int _getWidth();
	int _getHeight();

	void _initializeLibrary();
	void _initializeGlfwWindow();
	void _initializeCallbackAdapter();
	void _initializeImage();
	void _renderImage(PointCloud3d pointCloud, CameraModel cameraModel, ProjectionModel projectionModel);
	void _showImage();
	void _pollInteractionsWithWindow();
	void _setProjektionMatrixAccordingTo(ProjectionModel projectionModel);
	void _setCameraTransformation(Point3d center, CameraModel cameraModel);
	void _setupViewportMatrix();
};