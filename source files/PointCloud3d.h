#pragma once

#include <vector>
#include <algorithm>
#include "Point3d.h"
#include "ThreeDTree.h"

class PointCloud3d
{
public:
	PointCloud3d();
	~PointCloud3d();

	void setPointsTo(std::vector<Point3d> points);
	std::vector<Point3d>& getPoints();
	ThreeDTree& getTree();
	Point3d getCenter();
	double getRadius();
	std::vector<Point3d> query(Point3d const & referencePoint, double maximumDistance);
	PointCloud3d smooth( const double radius);
	void computeTree();

	// TODO: extent / bounding box, center
	// TODO: kd-tree, point representation using kd-tree
private:
	std::vector<Point3d> _points;
	ThreeDTree _tree;
	std::pair<Point3d, Point3d> _minMax;
	Point3d _sceneCenter;
	double _sceneRadius;

	void _computeCenter();
	void _computeRadius();
	void _computeBoundingBox();
};