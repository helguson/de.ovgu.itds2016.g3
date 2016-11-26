#pragma once

#include "Node.h"
#include <memory>
#include <vector>
#include <stack>
#include "Point3d.h"
#include "LeafNode.h"

class ThreeDTree {
public:
	void buildFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd);
	std::vector<Point3d> query(Interval3d const & range);
	std::vector<Point3d> query(Point3d const & referencePoint, double maximumDistance);
	std::shared_ptr<LeafNode> estimateLeafNodeNearOf(Point3d const & referencePoint) const;
	// returns first leaf node with a point that has the minimal distance
	std::shared_ptr<LeafNode> getLeafNodeNearestTo(Point3d const & referencePoint) const;
	static void runTests();

private:
	std::shared_ptr<Node> _root;

	// returns nullPtr if no nearer leafNode was found
	std::shared_ptr<LeafNode> _findLeafNodeNearerTo(Point3d const & referencePoint, std::shared_ptr<LeafNode> currentNearLeafNode) const;
};