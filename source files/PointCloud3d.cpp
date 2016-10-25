#include "PointCloud3d.h"

PointCloud3d::PointCloud3d()
	:_points()
{

}
PointCloud3d::~PointCloud3d() {

}

void PointCloud3d::setPointsTo(std::vector<Point3d> points) {

	this->_points = points;
}

std::vector<Point3d> PointCloud3d::getPoints() {
	
	return this->_points;
}