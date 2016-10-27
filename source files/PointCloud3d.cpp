#include "PointCloud3d.h"

PointCloud3d::PointCloud3d()
	:
	_points(),
	_sceneCenter(),
	_sceneRadius(),
	minMax()
{

}
PointCloud3d::~PointCloud3d() {

}

void PointCloud3d::setPointsTo(std::vector<Point3d> points) {

	this->_points = points;
	this->_computeBoundingBox();
	this->_computeCenter();
	this->_computeRadius();
}

std::vector<Point3d> PointCloud3d::getPoints() {
	
	return this->_points;
}

Point3d PointCloud3d::getCenter() {

	return this->_sceneCenter;
}

double PointCloud3d::getRadius() {

	return this->_sceneRadius;
}

void PointCloud3d::_computeCenter() {

	// compute center of scene
	this->_sceneCenter = (this->minMax.first + this->minMax.second) * 0.5;
}

void PointCloud3d::_computeRadius()
{
	this->_sceneRadius = distance3d(minMax.second, _sceneCenter);
}

void PointCloud3d::_computeBoundingBox()
{
	Point3d min = this->_points.front();
	Point3d max = this->_points.front();
	if (!this->_points.empty()) {
		for (unsigned int i = 0; i < this->_points.size(); i++) {
			const Point3d& point = this->_points[i];
			if (point.x < min.x) min.x = point.x;
			if (point.y < min.y) min.y = point.y;
			if (point.z < min.z) min.z = point.z;
			if (point.x > max.x) max.x = point.x;
			if (point.y > max.y) max.y = point.y;
			if (point.z > max.z) max.z = point.z;
		}
	}
	this->minMax = std::pair<Point3d, Point3d>(min,max);
}
