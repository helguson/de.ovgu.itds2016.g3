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
	void setOnRequestThinCloud(std::function<void() > callback);
	void setOnRequestSmoothCloud(std::function<void() > callback);
	void render(std::vector<std::shared_ptr<RenderableObjects>>& objects, ModelProperties& props);
	void render(double r, double g, double b);

	SettingsContainer getSettings() { return _window.getSettings(); };
	std::vector<std::string> getSelectedNames() { return _window.getSelectedNames(); };

private:
	QApplication _qApplication;
	MainWindow _window;
};