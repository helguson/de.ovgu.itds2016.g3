#pragma once

#include "Node.h"
#include <memory>
#include <vector>
#include "Point3d.h"

class ThreeDTree {
public:
	void buildFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd);
	std::vector<Point3d> query(Interval3d const & range);
	// std::vector<Point3D> query(const Point3d point, const double distance) const;
	// Point3D estimateNearNeighborOf(const Point3d point) const;

private:
	std::shared_ptr<Node> _root;
};