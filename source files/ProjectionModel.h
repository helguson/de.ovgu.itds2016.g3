#pragma once

class ProjectionModel {
public:
	ProjectionModel(double fieldOfViewAngleInYDirection = 45, double nearClippingPlaneZ = 0.001, double farClippingPlaneZ = 100);

	double getFieldOfViewAngleInYDirection();
	double getNearClippingPlaneZ();
	double getFarClippingPlaneZ();
	double setFieldOfViewAngleInYDirectionTo(double angle);
	double setNearClippingPlaneZTo(double z);
	double setFarClippingPlaneZTo(double z);

private:
	double _fieldOfViewAngleInYDirection;
	double _nearClippingPlaneZ;
	double _farClippingPlaneZ;
};