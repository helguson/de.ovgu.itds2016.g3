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
		[&pointString](Point3d& const point)->void {
			pointString += " " + point.toStr();
		}
	);

	log("(0,0,0) and (1,0,0) shoulb be found:" + pointString);
}

void ThreeDTree::runTests() {

	test01_query_interval3d();
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
			// handle inner node
			std::shared_ptr<InnerNode> currentInnerNode = std::static_pointer_cast<InnerNode>(currentNode);
			// TODO:
			// ---if range contains represented interval 
				// ---add all points of subtree to valid points
			if (range.intersectsWith(currentInnerNode->volume)) {
				// add children to be visited
				visitNeedingNodes.push(currentInnerNode->superiorChild);
				visitNeedingNodes.push(currentInnerNode->inferiorChild);
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

