#include "mainwindow.h"
#include <QtWidgets>
#include <iostream>

MainWindow::MainWindow()
{

	QWidget* widgetPtr = new QWidget;
	this->setCentralWidget(widgetPtr);

	this->_oglWidgetPtr = new OGLWidget;
	this->_oglWidgetPtr->setSizePolicy(
		QSizePolicy::Expanding,
		QSizePolicy::Expanding
	);

	QVBoxLayout* layoutPtr = new QVBoxLayout;
	layoutPtr->setMargin(5);
	layoutPtr->addWidget(this->_oglWidgetPtr);

	widgetPtr->setLayout(layoutPtr);

	this->_createActions();
	this->_createMenus();

	this->setWindowTitle(tr("I3DS"));
	this->setMinimumSize(160, 160);
	this->resize(600, 400);
	
}

void MainWindow::editSettings()
{
	SettingsWindow setWin(this);
	setWin.exec();
	_settings = setWin.getContainer();
}

void MainWindow::loadFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\Windows\\", tr("xyz Files (*.xyz)"));
	if (!fileName.isNull()) {
		this->_currFile = fileName.toStdString();
	}
	// inform Controller:
	this->_onRequestLoadFile(this->_currFile);

}

std::string MainWindow::getCurrentFile() {
	return this->_currFile;
}

void MainWindow::render(PointCloud3d& pointCloud, ModelProperties& props) {

		//send render info to widget
		this->_oglWidgetPtr->render(pointCloud, props, this->_settings);
		//this->_pollInteractionsWithWindow();
}

void MainWindow::render(int r, int g, int b) {

	//send render info to widget
	this->_oglWidgetPtr->render(r,g,b);
	//this->_pollInteractionsWithWindow();
}

void MainWindow::setOnRequestLoadFile(std::function<void(std::string)> callback)
{
	this->_onRequestLoadFile = callback;
}

void MainWindow::setOnRequestPaintGL(std::function<void()> callback)
{
	this->_oglWidgetPtr->setOnRequestPaintGL(callback);
}

void MainWindow::closeApplication() {
	std::cout << "invoked closeApplication" << std::endl;
}

void MainWindow::_createActions() {

	this->_createLoadFileAction();
	this->_createCloseApplicationAction();
	this->_createEditSettingsAction();
}

void MainWindow::_createLoadFileAction() {

	this->loadFileAction = new QAction(tr("&load file"), this);
	// TODO: setShortcuts
	this->loadFileAction->setStatusTip(tr("TODO"));
	this->connect(
		this->loadFileAction,
		&QAction::triggered,
		this,
		&MainWindow::loadFile
	);
}

void MainWindow::_createCloseApplicationAction() {

	this->closeApplicationAction = new QAction(tr("&close application"), this);
	// TODO: setShortcuts
	this->closeApplicationAction->setStatusTip(tr("TODO"));
	this->connect(
		this->closeApplicationAction,
		&QAction::triggered,
		this,
		&MainWindow::closeApplication
	);
}

void MainWindow::_createEditSettingsAction() {

	this->editSettingsAction = new QAction(tr("&edit Settings"), this);
	this->editSettingsAction->setStatusTip(tr("TODO"));
	this->connect(
		this->editSettingsAction,
		&QAction::triggered,
		this,
		&MainWindow::editSettings
	);
}

void MainWindow::_createMenus() {
	this->fileMenu = this->menuBar()->addMenu(tr("&File"));
	this->fileMenu->addAction(this->loadFileAction);
	this->fileMenu->addSeparator();
	this->fileMenu->addAction(this->closeApplicationAction);

	this->settingsMenu = this->menuBar()->addMenu(tr("&Settings"));
	this->settingsMenu->addAction(this->editSettingsAction);
}

//void MainWindow::_pollInteractionsWithWindow() {
//
//	// Poll for and process events
//	glfwPollEvents();
//}
//
//void MainWindow::setOnScrollCallbackTo(std::function<void(double, double)> callback) {
//
//	this->_callbackAdapter.setOnMouseWheelCallbackTo(
//		[callback](GLFWwindow* windowPtr, double offsetX, double offsetY)->void { callback(offsetX, offsetY); }
//	);
//}
//
//void MainWindow::setOnKeyCallbackTo(std::function<void(GLFWwindow*, int, int, int, int)> callback) {
//
//	this->_callbackAdapter.setOnKeyCallbackTo(callback);
//}
//


