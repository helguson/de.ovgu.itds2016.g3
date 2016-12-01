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

	this->_smoothBtPtr = new QPushButton;
	this->_smoothBtPtr->setText(tr("Smoothing"));
	this->_smoothBtPtr->setMaximumHeight(30);
	this->_smoothBtPtr->setMaximumWidth(100);

	this->_thinBtPtr = new QPushButton;
	this->_thinBtPtr->setText(tr("Thinning"));
	this->_thinBtPtr->setMaximumHeight(30);
	this->_thinBtPtr->setMaximumWidth(100);

	QVBoxLayout* settingsLayoutPtr = new QVBoxLayout;
	settingsLayoutPtr->setAlignment(Qt::AlignTop);
	settingsLayoutPtr->addWidget(this->_thinBtPtr);
	settingsLayoutPtr->addWidget(this->_smoothBtPtr);

	QHBoxLayout* layoutPtr = new QHBoxLayout;
	layoutPtr->setMargin(5);
	layoutPtr->addWidget(this->_oglWidgetPtr);
	layoutPtr->addLayout(settingsLayoutPtr);

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

void MainWindow::thinCloud(){
}

void MainWindow::smoothCloud() {

}
std::string MainWindow::getCurrentFile() {
	return this->_currFile;
}

void MainWindow::render(std::vector<std::shared_ptr<PointCloud3d>>& pointCloud, ModelProperties& props) {

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
	this->_createSmoothAction();
	this->_createThinAction();
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

void MainWindow::_createSmoothAction() {

	this->smoothAction = new QAction(tr("&edit Settings"), this);
	this->smoothAction->setStatusTip(tr("TODO"));
	this->connect(
		this->smoothAction,
		&QAction::triggered,
		this,
		&MainWindow::smoothCloud
	);
}

void MainWindow::_createThinAction() {

	this->thinAction = new QAction(tr("&edit Settings"), this);
	this->thinAction->setStatusTip(tr("TODO"));
	this->connect(
		this->thinAction,
		&QAction::triggered,
		this,
		&MainWindow::thinCloud
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


