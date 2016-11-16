#include "ThreeDTree.h"
#include <algorithm>
#include "LeafNode.h"
#include "ThreeDTreeStructureBuilder.h"
#include "testHelpers.h"


void test01_query_interval3d() {
	log("####################");
	log("### start test01 ###");
	log("####################");


	// setup
	//
	//     -1  0  1  2  x
    //  1   o
	//     
	//  0      o  o  o
    // 
	// -1      o
	//   
	//  y


	std::vector<Point3d> data;
	data.push_back(Point3d(-1, 1, 0));
	data.push_back(Point3d(0, 0, 0));
	data.push_back(Point3d(1, 0, 0));
	data.push_back(Point3d(2, 0, 0));
	data.push_back(Point3d(0, -1, 0));

	Interval3d range(
		Interval(0, true, 1, true),	// x [0, 1]
		Interval(-1, false, 1, false), // y (-1, 1)
		Interval(0, true, 0, true)  // z [0, 0]
	);

	ThreeDTree tree;
	tree.buildFor(data.begin(), data.end());

	std::vector<Point3d> queryResult = tree.query(range);

	std::string pointString;
	std::for_each(
		queryResult.begin(), queryResult.end(),
		[&pointString](Point3d const & point)->void {
			pointString += " " + point.toStr();
		}
	);

	log("(0,0,0) and (1,0,0) should be found:" + pointString);
}

void test02_query_Point3dWidthMaximumDistance() {
	log("####################");
	log("### start test02 ###");
	log("####################");


	// setup
	//
	//     -1  0  1  2  3  4  x
	//  1   o    ---
	//         /  o  \
	//  0     /       \
	//       /         \
	// -1   |  o  *     |o
	//       \         /
	// -2     \       /
    //         \     /
	// -3        ---
	//   
	//  y


	Point3d referencePoint(1, -1, 0);
	double maximumDistance = 2;

	double mu = 0.1;

	std::vector<Point3d> data;
	data.push_back(Point3d(-1, 1, 0));
	data.push_back(Point3d(1, 1-mu, 0));
	data.push_back(Point3d(0, -1, 0));
	data.push_back(Point3d(3+mu, -1, 0));

	ThreeDTree tree;
	tree.buildFor(data.begin(), data.end());

	std::vector<Point3d> queryResult = tree.query(referencePoint, maximumDistance);

	std::string pointString;
	std::for_each(
		queryResult.begin(), queryResult.end(),
		[&pointString](Point3d const & point)->void {
		pointString += " " + point.toStr();
	}
	);

	log("(1,1-mu,0) and (0,-1,0) should be found:" + pointString);
}

void ThreeDTree::runTests() {

	test01_query_interval3d();
	test02_query_Point3dWidthMaximumDistance();
}

void ThreeDTree::buildFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd) {

	size_t size = std::distance(dataBegin, dataEnd);

	if (size >= 1) {
		ThreeDTreeStructureBuilder builder;
		this->_root = builder.buildStructureFor(dataBegin, dataEnd);
	}
	else {
		this->_root = nullptr;
	}
}

std::vector<Point3d> ThreeDTree::query(Interval3d const & range) {

	std::vector<Point3d> validPoints;
	std::vector<Point3d> possiblyValidPoints;

	std::stack<std::shared_ptr<Node>> visitNeedingNodes;

	// travers structure
	// init
	visitNeedingNodes.push(this->_root);

	// add possibly valid and valid points skipping impossible branches
	while (!visitNeedingNodes.empty()) { // there is something
		std::shared_ptr<Node> currentNode = visitNeedingNodes.top();
		visitNeedingNodes.pop();

		if (currentNode->isLeafNode()) {
			// handle leaf
			std::shared_ptr<LeafNode> currentLeafNode = std::static_pointer_cast<LeafNode>(currentNode);
			if (range.intersectsWith(currentLeafNode->volume)) {
				possiblyValidPoints.push_back(*currentLeafNode->pointPtr);
			}
		}
		else {
			
			// ---if range contains represented interval
			if (range.contains(currentNode->volume)) {
				addAllChildren(currentNode, &validPoints); 
			}
			else {
				// handle inner node
				std::shared_ptr<InnerNode> currentInnerNode = std::static_pointer_cast<InnerNode>(currentNode);
				// ---add all points of subtree to valid points
				if (range.intersectsWith(currentInnerNode->volume)) {
					// add children to be visited
					visitNeedingNodes.push(currentInnerNode->superiorChild);
					visitNeedingNodes.push(currentInnerNode->inferiorChild);
				}
			}
		}
	}

	// test possibly valid points for validity
	std::for_each(
		possiblyValidPoints.begin(), possiblyValidPoints.end(),
		[&validPoints, &range](Point3d& point)->void {
			if (range.contains(point)) {
				validPoints.push_back(point);
			}
		}
	);

	return validPoints;
}

std::vector<Point3d> ThreeDTree::query(Point3d const & referencePoint, double maximumDistance) {

	std::vector<Point3d> validPoints;

	Interval3d hullRange(
		Interval(referencePoint.x - maximumDistance, true, referencePoint.x + maximumDistance, true),
		Interval(referencePoint.y - maximumDistance, true, referencePoint.y + maximumDistance, true),
		Interval(referencePoint.z - maximumDistance, true, referencePoint.z + maximumDistance, true)
	);

	std::vector<Point3d> possiblyValidPoints = this->query(hullRange);

	// test possibly valid points for validity
	std::for_each(
		possiblyValidPoints.begin(), possiblyValidPoints.end(),
		[&validPoints, &referencePoint, &maximumDistance](Point3d& possiblyValidPoint)->void {
			if (distance3d(referencePoint, possiblyValidPoint) <= maximumDistance) {
				validPoints.push_back(possiblyValidPoint);
			}
		}
	);

	return validPoints;
}

Point3d ThreeDTree::estimateNearNeighborOf(const Point3d point) const
{
	return Point3d();
}

void ThreeDTree::addAllChildren(std::shared_ptr<Node> node, std::vector<Point3d>* validPoints) {
	std::stack<std::shared_ptr<Node>> stack;
	stack.push(node);

	while (!stack.empty()) {
		std::shared_ptr<Node> tmpNode = stack.top();
		stack.pop();
		if (tmpNode->isLeafNode()) {
			std::shared_ptr<LeafNode> leafNode = std::static_pointer_cast<LeafNode>(tmpNode);
			validPoints->push_back(*leafNode->pointPtr);
		}
		else {
			std::shared_ptr<InnerNode> innerNode = std::static_pointer_cast<InnerNode>(tmpNode);
			stack.push(innerNode->superiorChild);
			stack.push(innerNode->inferiorChild);
		}
	}
}



