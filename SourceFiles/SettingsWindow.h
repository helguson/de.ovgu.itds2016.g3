#pragma once

#include <QDialog>
#include "SettingsContainer.h"
#include "ui_SettingsWindow.h"

class SettingsWindow : public QDialog
{
	Q_OBJECT

public:
	SettingsWindow(QWidget *parent = Q_NULLPTR);
	~SettingsWindow();
	SettingsContainer getContainer();
private:
	SettingsContainer _container;
	Ui::SettingsWindow ui;
private slots:
	void setNNRadius(double value);
	void setShowQuery(int state);

};
