#pragma once

#include <QApplication>
#include "MainWindow.h"

class QtView
{
public:
	QtView(int numberOfArguments, char** arguments);

	int startApplication();

	void setOnRequestPaintGL(std::function<void()> callback);
	void setOnRequestLoadFile(std::function<void(std::string)> callback);
	void render(std::vector<std::shared_ptr< PointCloud3d>>& cloud, ModelProperties& props);
	void render(double r, double g, double b);

private:
	QApplication _qApplication;
	MainWindow _window;
};