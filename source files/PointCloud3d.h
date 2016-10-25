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

	// TODO: extent / bounding box, center
private:
	std::vector<Point3d> _points;
};