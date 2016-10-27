#pragma once

#include <vector>
#include "Point3d.h"

class PointCloud3d
{
public:
	PointCloud3d();
	~PointCloud3d();

	void setPointsTo(std::vector<Point3d> points);
	std::vector<Point3d> getPoints();
	Point3d getCenter();
	double getRadius();

	// TODO: extent / bounding box, center
	// TODO: kd-tree, point representation using kd-tree
private:
	std::vector<Point3d> _points;
	std::pair<Point3d, Point3d> minMax;
	Point3d _sceneCenter;
	double _sceneRadius;

	void _computeCenter();
	void _computeRadius();
	void _computeBoundingBox();
};