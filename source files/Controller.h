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
	bool shouldContinueLooping();

private:
	GlfwWindowView _view;
	Model _model;
	FileLoader _fileLoader;
};