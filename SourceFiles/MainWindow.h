#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <functional>
#include <string>
#include "OGLWidget.h"
#include "SettingsContainer.h"
#include "ModelProperties.h"
#include "PointCloud3d.h"
#include "SettingsWindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	std::string getCurrentFile();
	bool isCorrectlyInitialized();
	void renderData(PointCloud3d& cloud, ModelProperties props);
	void setOnNewFileLoaded(std::function<void(std::string )> callback);
	void setOnPaintRequest(std::function<void() > callback);
	//void setOnKeyCallbackTo(std::function<void(GLFWwindow*, int, int, int, int)> callback);

private:
	std::string _currFile;
    Ui::MainWindowClass ui;
	OGLWidget _oglView;
	SettingsContainer _settings;
	bool _isWindowInitialized;

	void _initializeWindow();
	//void _pollInteractionsWithWindow();

	//Callbacks:
	std::function<void(std::string)> newFileLoaded;

private slots:
	void loadFile();
	SettingsContainer editSettings();
	//void setOnScrollCallbackTo(std::function<void(double, double)> callback);
	};
