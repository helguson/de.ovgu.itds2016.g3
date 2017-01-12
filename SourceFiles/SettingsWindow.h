#pragma once

#include <QDialog>
#include <QColorDialog>
#include "SettingsContainer.h"
#include "ui_SettingsWindow.h"

class SettingsWindow : public QDialog
{
	Q_OBJECT

public:
	SettingsWindow(QWidget *parent = Q_NULLPTR);
	~SettingsWindow();
	SettingsContainer getContainer();
	void loadCurrent(SettingsContainer container);
private:
	SettingsContainer _container;
	Ui::SettingsWindow ui;
private slots:
	void setNNColor();
	void setPCColor();
	void setSCColor();
	void setTCColor();
	void setShowQuery(int state);
	void setShowDistance(int state);

};
