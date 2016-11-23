#pragma once

class ProjectionModel {
public:
	ProjectionModel(double fieldOfViewAngleInYDirection = 45, double nearClippingPlaneZ = 0.001, double farClippingPlaneZ = 100);

	double getFieldOfViewAngleInYDirection();
	double getNearClippingPlaneZ();
	double getFarClippingPlaneZ();

	void setFieldOfViewAngleInYDirectionTo(double angle);
	void setNearClippingPlaneZTo(double z);
	void setFarClippingPlaneZTo(double z);
private: 
	double _fieldOfViewAngleInYDirection;
	double _nearClippingPlaneZ;
	double _farClippingPlaneZ;
};