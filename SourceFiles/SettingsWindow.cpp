#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
}

SettingsContainer SettingsWindow::getContainer() {
	return this->_container;
}

void SettingsWindow::setShowQuery(int state)
{
	this->_container.showQuery = state > 0;
}

void SettingsWindow::setNNRadius(double value) {
	this->_container.nnRadius = value;
}

