#pragma once

#include "Node.h"
#include <memory>
#include <vector>
#include <stack>
#include "Point3d.h"

class ThreeDTree {
public:
	void buildFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd);
	std::vector<Point3d> query(Interval3d const & range);
	std::vector<Point3d> query(Point3d const & referencePoint, double maximumDistance);
	Point3d estimateNearNeighborOf(const Point3d point) const;
	void addAllChildren(std::shared_ptr<Node> currentNode, std::vector<Point3d>* nodeStack);
	static void runTests();

private:
	std::shared_ptr<Node> _root;
};