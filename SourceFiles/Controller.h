#pragma once

#include "Model.h"
#include "FileLoader.h"
#include "MainWindow.h"
#include "QtView.h"
#include <functional>
#include <iostream>

class Controller
{
public:
	Controller( int numberOfArguments, char** arguments);
	~Controller();

	int startApplication();
private:
	QtView _view;
	Model _model;

};