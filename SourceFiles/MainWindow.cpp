#include "mainwindow.h"
#include <QtWidgets>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	_oglView(parent),
	_isWindowInitialized(false)
{
	ui.setupUi(this);
	this->_initializeWindow();
}

bool MainWindow::isCorrectlyInitialized() {

	return this->_isWindowInitialized;
}


void MainWindow::_initializeWindow() {
	
	this->setWindowTitle("I3DS");
	this->setMinimumSize(600, 400);
	this->resize(600, 400);

	this->_isWindowInitialized = true;
}

SettingsContainer MainWindow::editSettings()
{
	SettingsWindow setWin(this);
	setWin.exec();
	return setWin.getContainer();
}

void MainWindow::loadFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\Windows\\", tr("xyz Files (*.xyz)"));
	if (!fileName.isNull()) {
		this->_currFile = fileName.toStdString();
	}
	// inform Controller:
	this->newFileLoaded(this->_currFile);
}

std::string MainWindow::getCurrentFile() {
	return this->_currFile;
}

void MainWindow::renderData(PointCloud3d& pointCloud, ModelProperties props) {

		//send render info to widget
		this->_oglView.renderData(pointCloud, props, this->_settings);
		//this->_pollInteractionsWithWindow();
}

void MainWindow::setOnNewFileLoaded(std::function<void(std::string)> callback)
{
	this->newFileLoaded = callback;
}

void MainWindow::setOnPaintRequest(std::function<void()> callback)
{
	this->_oglView.setOnPaintRequest(callback);
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


