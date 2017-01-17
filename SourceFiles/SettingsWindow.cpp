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

void SettingsWindow::loadCurrent(SettingsContainer container)
{
	_container = container;
	
	QString qss = QString("background-color: %1").arg(this->_container.NNColor.name());
	ui.pBNNColor->setStyleSheet(qss);
	
	qss = QString("background-color: %1").arg(this->_container.PCColor.name());
	ui.pBPCColor->setStyleSheet(qss);
	
	qss = QString("background-color: %1").arg(this->_container.SCColor.name());
	ui.pBSCColor->setStyleSheet(qss);
	
	qss = QString("background-color: %1").arg(this->_container.TCColor.name());
	ui.pBTCColor->setStyleSheet(qss);

	ui.Distcb->setChecked(this->_container.showDistanceToPlane);
	ui.NNcb->setChecked(this->_container.showQuery);
}

void SettingsWindow::setShowQuery(int state)
{
	this->_container.showQuery = state > 0;
}

void SettingsWindow::setShowDistance(int state)
{
	this->_container.showDistanceToPlane = state > 0;
}

void SettingsWindow::setNNColor() {
	this->_container.NNColor = QColorDialog::getColor();
	QString qss = QString("background-color: %1").arg(this->_container.NNColor.name());
	ui.pBNNColor->setStyleSheet(qss);
}
void SettingsWindow::setPCColor() {
	this->_container.PCColor = QColorDialog::getColor();
	QString qss = QString("background-color: %1").arg(this->_container.PCColor.name());
	ui.pBPCColor->setStyleSheet(qss);
}
void SettingsWindow::setSCColor() {
	this->_container.SCColor = QColorDialog::getColor();
	QString qss = QString("background-color: %1").arg(this->_container.SCColor.name());
	ui.pBSCColor->setStyleSheet(qss);
}
void SettingsWindow::setTCColor() {
	this->_container.TCColor = QColorDialog::getColor();
	QString qss = QString("background-color: %1").arg(this->_container.TCColor.name());
	ui.pBTCColor->setStyleSheet(qss);
}

