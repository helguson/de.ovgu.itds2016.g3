#pragma once

#include "GlfwWindowView.h"
#include "Model.h"
#include "FileLoader.h"

class Controller
{
public:
	Controller();
	~Controller();

	void startMainLoop();
	// for internal use only
	// TODO: search for better solution
	void stopMainLoop();
	bool shouldDoNextLoop();

private:
	GlfwWindowView _view;
	Model _model;
	FileLoader _fileLoader;
	bool _shouldContiniueLooping;

	std::function<void(std::shared_ptr<std::vector<Point3d>>)> Controller::_getStoreCreatedDataFunctor();
};