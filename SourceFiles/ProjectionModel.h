#pragma once
#include <qmatrix4x4.h>

class ProjectionModel {
public:
	ProjectionModel(double fieldOfViewAngleInYDirection = 45, double nearClippingPlaneZ = 0.001, double farClippingPlaneZ = 100);

	double getFieldOfViewAngleInYDirection();
	double getNearClippingPlaneZ();
	double getFarClippingPlaneZ();
	double getAspectRatio();

	void setFieldOfViewAngleInYDirectionTo(double angle);
	void setNearClippingPlaneZTo(double z);
	void setFarClippingPlaneZTo(double z);
	void setWindowSize(double width, double height);
	QMatrix4x4 getProjektionMatrix();

private: 
	double _fieldOfViewAngleInYDirection;
	double _nearClippingPlaneZ;
	double _farClippingPlaneZ;
	double _height, _width;
};