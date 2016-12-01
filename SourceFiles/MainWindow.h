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
	void render(int r, int g, int b);
	void setOnRequestLoadFile(std::function<void(std::string)> callback);
	void setOnRequestPaintGL(std::function<void() > callback);
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
	//void _pollInteractionsWithWindow();

	void _createActions();
	void _createMenus();
	void _createLoadFileAction();
	void _createCloseApplicationAction();
	void _createEditSettingsAction();
	void _createSmoothAction();
	void _createThinAction();

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
