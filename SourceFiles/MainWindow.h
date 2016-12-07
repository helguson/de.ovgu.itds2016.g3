#pragma once

#include <QtWidgets/QMainWindow>
#include <functional>
#include <string>
#include <QtWidgets>
#include <iostream>
#include <QListWidget>
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
	void render(std::vector<std::shared_ptr<PointCloud3d>>& visibleElements);
	void render(int r, int g, int b);
	void setOnRequestLoadFile(std::function<void(std::string)> callback);
	void setOnRequestPaintGL(std::function<void() > callback);
	void setOnRequestThinCloud(std::function<void() > callback);
	void setOnRequestSmoothCloud(std::function<void() > callback);
	void setOnRequestUpdateOGLWidget(std::function<void() > callback);
	void setOnRequestScroll(std::function<void(double, double)> callback);
	void addVisibleElementToList();
	void updateProjectionModelView(ModelProperties& props);
	SettingsContainer getSettings() { return _settings; };
	std::vector<int> getVisibleElementsIndices();
	//void setOnKeyCallbackTo(std::function<void(GLFWwindow*, int, int, int, int)> callback);

private:
	OGLWidget* _oglWidgetPtr;
	SettingsContainer _settings;

	// buttons 
	QPushButton* _smoothBtPtr;
	QPushButton* _thinBtPtr;

	// SpinnBoxes
	QDoubleSpinBox* _smoothFactorSbPtr;
	QDoubleSpinBox* _thinRadiusSbPtr;

	//List of visible Elements
	QListWidget* _visibleElementsScrollWidgetPtr;

	//callbacks
	std::function<void(std::string)> _onRequestLoadFile;
	std::function<void()> _onRequestSmoothCloud;
	std::function<void()> _onRequestThinCloud;
	std::function<void()> _onRequestUpdateOGLWidget;
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
	void changeSmoothFactor(double value);
	void changeThinRadius(double value);
	void repaintOGLWidget(QListWidgetItem* sender);
	//void setOnScrollCallbackTo(std::function<void(double, double)> callback);
	};
