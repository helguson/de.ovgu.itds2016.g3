#pragma once

#include <QtWidgets/QMainWindow>
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
    MainWindow();
	std::string getCurrentFile();
	void render(std::vector<std::shared_ptr<PointCloud3d>>& cloud, ModelProperties& props);
	void render(double r, double g, double b);
	void setOnRequestLoadFile(std::function<void(std::string)> callback);
	void setOnRequestPaintGL(std::function<void() > callback);
	void setOnRequestThinCloud(std::function<void() > callback);
	void setOnRequestSmoothCloud(std::function<void() > callback);
	SettingsContainer getSettings() { return _settings; };
	//void setOnKeyCallbackTo(std::function<void(GLFWwindow*, int, int, int, int)> callback);

private:
	std::string _currFile;
	OGLWidget* _oglWidgetPtr;
	SettingsContainer _settings;

	// buttons
	QPushButton* _smoothBtPtr;
	QPushButton* _thinBtPtr;

	//callbacks
	std::function<void(std::string)> _onRequestLoadFile;
	std::function<void()> _onRequestSmoothCloud;
	std::function<void()> _onRequestThinCloud;
	//void _pollInteractionsWithWindow();

	void _createActions();
	void _createMenus();
	void _createLoadFileAction();
	void _createCloseApplicationAction();
	void _createEditSettingsAction();

	// menus
	QMenu* fileMenu;
	QMenu* settingsMenu;

	// file menu actions
	QAction* loadFileAction;
	QAction* closeApplicationAction;

	//buttons actions
	QAction* smoothAction;
	QAction* thinAction;

	//settings actions
	QAction* editSettingsAction;

	// help menu actions
	QAction* showHelpAction;
	QAction* showInformationAboutThisApplicationAction;


private slots:
	void loadFile();
	void editSettings();
	void closeApplication();
	void thinCloud();
	void smoothCloud();
	//void setOnScrollCallbackTo(std::function<void(double, double)> callback);
	};
