#pragma once

#include "Model.h"
#include "FileLoader.h"
#include "MainWindow.h"
#include <functional>
#include <iostream>

class Controller
{
public:
	Controller(int argc, char* argv[]);
	~Controller();

private:
	MainWindow _view;
	Model _model;
	void renderData();

};