#include "ThreeDTree.h"
#include <algorithm>
#include "LeafNode.h"
#include "ThreeDTreeStructureBuilder.h"

#include "testHelpers.h"
#include "util.h"


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
	std::shared_ptr<std::vector<Point3d*>> dataPtrs = util::computePointPtrVectorFrom(data);

	ThreeDTree tree(*dataPtrs);

	Interval3d range(
		Interval(0, true, 1, true),	// x [0, 1]
		Interval(-1, false, 1, false), // y (-1, 1)
		Interval(0, true, 0, true)  // z [0, 0]
	);

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> queryResult = tree.query(range).leafNodePtrs;

	std::string pointString;
	std::for_each(
		queryResult->begin(), queryResult->end(),
		[&pointString](std::shared_ptr<LeafNode> leafNode)->void {
			pointString += " " + leafNode->pointPtr->toStr();
		}
	);

	log("(0,0,0) and (1,0,0) should be found:" + pointString);
}

void test02_query_interval3d_2() {
	log("####################");
	log("### start test02 ###");
	log("####################");


	// setup
	//
	//     -1  0  1  x
	//  3   o
	//     
	//  2      o
	// 
	//  1         o
	//
	//  0         o
	//   
	//  y


	std::vector<Point3d> data;
	data.push_back(Point3d(-1, 3, 0));
	data.push_back(Point3d(0, 2, 0));
	data.push_back(Point3d(1, 1, 0));
	data.push_back(Point3d(1, 0, 0));
	std::shared_ptr<std::vector<Point3d*>> dataPtrs = util::computePointPtrVectorFrom(data);

	ThreeDTree tree(*dataPtrs);

	Interval3d range(
		Interval(1, true, 1, true),	// x [1, 1]
		Interval(0, true, 1, true), // y [0, 1]
		Interval(0, true, 0, true)  // z [0, 0]
	);

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> queryResult = tree.query(range).leafNodePtrs;

	std::string pointString;
	std::for_each(
		queryResult->begin(), queryResult->end(),
		[&pointString](std::shared_ptr<LeafNode> leafNode)->void {
		pointString += " " + leafNode->pointPtr->toStr();
	}
	);

	log("(1,1,0) and (1,0,0) should be found:" + pointString);
}

void test03_query_Point3dWithMaximumDistance() {
	log("####################");
	log("### start test03 ###");
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
	std::shared_ptr<std::vector<Point3d*>> dataPtrs = util::computePointPtrVectorFrom(data);

	ThreeDTree tree(*dataPtrs);

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> queryResult = tree.query(referencePoint, maximumDistance).leafNodePtrs;

	std::string pointString;
	std::for_each(
		queryResult->begin(), queryResult->end(),
		[&pointString](std::shared_ptr<LeafNode> leafNode)->void {
		pointString += " " + leafNode->pointPtr->toStr();
	}
	);

	log("(1,1-mu,0) and (0,-1,0) should be found:" + pointString);
}

void test04_getLeafNodeNearestTo() {
	log("####################");
	log("### start test04 ###");
	log("####################");

	// setup
	//
	//     -5  -4  -3  -2  -1   0   1   2   3   x
	//  4                           o|
	//                              B|
	//  3                            |
	//                               |
	//  2                            |
	//      _________________________|
	//  1   o                        |  o
	//      A                        |  D
	//  0                       x    |
	//                               |_______
	// -1                            |      o
	//                                      C
	//  y
	//
	//
	// tree
	//        Bx
	//       / \
	//      /   \
	//     /     \
	//    Ay      Cy
	//   / \     / \
	//  A   B   C   D


	Point3d A(-5, 1, 0);
	Point3d B(1, 4, 0);
	Point3d C(3, -1, 0);
	Point3d D(2, 1, 0);

	Point3d referencePoint(0, 0, 0);



	double maximumDistance = 2;

	std::vector<Point3d> data;
	data.push_back(A);
	data.push_back(B);
	data.push_back(C);
	data.push_back(D);
	std::shared_ptr<std::vector<Point3d*>> dataPtrs = util::computePointPtrVectorFrom(data);

	ThreeDTree tree(*dataPtrs);

	std::shared_ptr<LeafNode> nearLeafNode = tree.estimateLeafNodeNearOf(referencePoint);
	std::shared_ptr<LeafNode> nearestLeafNode = tree.getLeafNodeNearestTo(referencePoint);

	log("near point estimation should be " + A.toStr() + " and is: " + nearLeafNode->pointPtr->toStr());
	log("nearest point should be " + D.toStr() + " and is: " + nearestLeafNode->pointPtr->toStr());
}

void test05_query_interval3d_hiddenPoint() {
	log("####################");
	log("### start test05 ###");
	log("####################");

	Point3d A(0, 0, 0);
	Point3d B(1, 0, 0);

	std::vector<Point3d> data;
	data.push_back(A);
	data.push_back(B);
	std::shared_ptr<std::vector<Point3d*>> dataPtrs = util::computePointPtrVectorFrom(data);

	ThreeDTree tree(*dataPtrs);

	std::shared_ptr<LeafNode> ALeaf = tree.getLeafNodeNearestTo(A);
	ALeaf->hide();

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> queryResult = tree.query(Interval3d::all()).leafNodePtrs;

	std::string pointString;
	std::for_each(
		queryResult->begin(), queryResult->end(),
		[&pointString](std::shared_ptr<LeafNode> leafNode)->void {
			pointString += " " + leafNode->pointPtr->toStr();
		}
	);

	log(B.toStr() + " should be found, found: " + pointString);
}

void test06_query_interval3d_hiddenInnerNode() {
	log("####################");
	log("### start test06 ###");
	log("####################");

	//   0   1   2   x
	// 2      |  o
	//        |  C
	// 1     o|
	//   ____B|
	// 0 o    |
	//   A    |
	// y
	//
	//      Bx
	//     / \
	//    Ay  C
	//   / \
	//  A   B
	//
	// hide Ay

	Point3d A(0, 0, 0);
	Point3d B(1, 1, 0);
	Point3d C(2, 2, 0);

	std::vector<Point3d> data;
	data.push_back(A);
	data.push_back(B);
	data.push_back(C);
	std::shared_ptr<std::vector<Point3d*>> dataPtrs = util::computePointPtrVectorFrom(data);

	ThreeDTree tree(*dataPtrs);

	std::shared_ptr<InnerNode> Ay = tree.query(Interval3d(
		Interval(-std::numeric_limits<double>::infinity(), false, B.x, true), //(-Inf, B.x]
		Interval::all(),
		Interval::all()
	)).innerNodePtrs->at(0);

	Ay->hide();

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> queryResult = tree.query(Interval3d::all()).leafNodePtrs;

	std::string pointString;
	std::for_each(
		queryResult->begin(), queryResult->end(),
		[&pointString](std::shared_ptr<LeafNode> leafNode)->void {
		pointString += " " + leafNode->pointPtr->toStr();
	}
	);

	log(C.toStr() + " should be found, found: " + pointString);
}

void ThreeDTree::runTests() {

	test01_query_interval3d();
	test02_query_interval3d_2();
	test03_query_Point3dWithMaximumDistance();
	test04_getLeafNodeNearestTo();
	test05_query_interval3d_hiddenPoint();
	test06_query_interval3d_hiddenInnerNode();
}


ThreeDTree::ThreeDTree(std::vector<Point3d*> const & dataPtrs)
	:_root(nullptr),
	_leafNodePtrs(nullptr)
{
	this->buildFor(dataPtrs);
}

void ThreeDTree::buildFor(std::vector<Point3d*> const & dataPtrs) {

	if (dataPtrs.size() >= 1) {
		ThreeDTreeStructureBuilder builder;
		this->_leafNodePtrs = builder.buildLeafNodesFor(dataPtrs);
		this->_root = builder.buildStructureFor(this->_leafNodePtrs);
	}
	else {
		this->_root = nullptr;
		this->_leafNodePtrs = nullptr;
	}
}

void ThreeDTree::rebuildStructure() {

	if (this->_leafNodePtrs != nullptr) {
		ThreeDTreeStructureBuilder builder;
		this->_root = builder.buildStructureFor(this->_leafNodePtrs);
	}
	else {
		this->_root = nullptr;
	}
}

// TODO: use _traverseTreeAt-function
void addAllChildren(std::shared_ptr<Node> node, std::vector<Point3d>* validPoints) {
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


ThreeDTree::QueryResult ThreeDTree::query(Interval3d const & range) const {

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> validLeafNodes
		= std::make_shared<std::vector<std::shared_ptr<LeafNode>>>();
	std::shared_ptr<std::vector<std::shared_ptr<InnerNode>>> validInnerNodes
		= std::make_shared<std::vector<std::shared_ptr<InnerNode>>>();
	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> possiblyValidLeafNodes
		= std::make_shared<std::vector<std::shared_ptr<LeafNode>>>();
	std::shared_ptr<std::vector<std::shared_ptr<InnerNode>>> possiblyValidInnerNodes // TODO: needed?
		= std::make_shared<std::vector<std::shared_ptr<InnerNode>>>();

	std::function<void(std::shared_ptr<LeafNode>, int, std::function<void()>)> handleLeafNode
		= [&possiblyValidLeafNodes, range](std::shared_ptr<LeafNode> nodePtr, int level, std::function<void()> abort)->void {
		if (range.intersectsWith(nodePtr->volume)) {
			possiblyValidLeafNodes->push_back(nodePtr);
		}
	};

	std::function<void(std::shared_ptr<InnerNode>, int, std::function<void()>)> handleInnerNode
		= [&validInnerNodes, range](std::shared_ptr<InnerNode> nodePtr, int level, std::function<void()> abort)->void {
		if (range.contains(nodePtr->volume)) {
			validInnerNodes->push_back(nodePtr);
		}
		// TODO:
		// ---if range contains represented interval 
		// ---add all points of subtree to valid points
	};
	
	std::function<bool(InnerNode const &, int)> canExcludeInferiorChildOf
		= [&range](InnerNode const & node, int level)->bool {
		return node.inferiorChild->isHidden()
			|| !range.intersectsWith(node.inferiorChild->volume);
	};
	
	std::function<bool(InnerNode const &, int)> canExcludeSuperiorChildOf
		= [&range](InnerNode const & node, int level)->bool {
		return node.superiorChild->isHidden()
			|| !range.intersectsWith(node.superiorChild->volume);
	};

	ThreeDTree::_traversTreeAt(
		this->_root,
		handleLeafNode,
		handleInnerNode,
		canExcludeInferiorChildOf,
		canExcludeSuperiorChildOf
	);

	// test possibly valid nodes for validity
	std::for_each(
		possiblyValidLeafNodes->begin(), possiblyValidLeafNodes->end(),
		[&validLeafNodes, &range](std::shared_ptr<LeafNode> leafNode)->void {
			if (range.contains(*leafNode->pointPtr)) {
				validLeafNodes->push_back(leafNode);
			}
		}
	);
	std::for_each(
		possiblyValidInnerNodes->begin(), possiblyValidInnerNodes->end(),
		[&validInnerNodes, &range](std::shared_ptr<InnerNode> innerNode)->void {
			if (range.contains(innerNode->volume)) {
				validInnerNodes->push_back(innerNode);
			}
		}
	);

	return QueryResult(validLeafNodes, validInnerNodes);
}

ThreeDTree::QueryResult ThreeDTree::query(Point3d const & referencePoint, double maximumDistance) const {

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> validLeafNodes
		= std::make_shared<std::vector<std::shared_ptr<LeafNode>>>();
	std::shared_ptr<std::vector<std::shared_ptr<InnerNode>>> validInnerNodes
		= std::make_shared<std::vector<std::shared_ptr<InnerNode>>>();
	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> possiblyValidLeafNodes;
	std::shared_ptr<std::vector<std::shared_ptr<InnerNode>>> possiblyValidInnerNodes;

	Interval3d hullRange = Interval3d::getSphereEncapsulatingIntervalFor(referencePoint, maximumDistance);
	QueryResult queryResult = this->query(hullRange);

	possiblyValidInnerNodes = queryResult.innerNodePtrs;
	possiblyValidLeafNodes = queryResult.leafNodePtrs;

	// test possibly valid nodes for validity
	std::for_each(
		possiblyValidLeafNodes->begin(), possiblyValidLeafNodes->end(),
		[&validLeafNodes, &referencePoint, &maximumDistance](std::shared_ptr<LeafNode> leafNode)->void {
			if (distance3d(referencePoint, *leafNode->pointPtr) <= maximumDistance) {
				validLeafNodes->push_back(leafNode);
			}
		}
	);
	std::for_each(
		possiblyValidInnerNodes->begin(), possiblyValidInnerNodes->end(),
		[&validInnerNodes, &referencePoint, &maximumDistance](std::shared_ptr<InnerNode> innerNode)->void {
			if (innerNode->volume.isContainedInSphereWith(referencePoint, maximumDistance)) {
				validInnerNodes->push_back(innerNode);
			}
		}
	);

	return QueryResult(validLeafNodes, validInnerNodes);
}

// TODO: use _traverseTreeAt-function (?)
std::shared_ptr<LeafNode> ThreeDTree::estimateLeafNodeNearOf(Point3d const & referencePoint) const {

	std::shared_ptr<Node> currentNode = this->_root;

	while (!currentNode->isLeafNode()) {
		std::shared_ptr<InnerNode> currentInnerNode = std::static_pointer_cast<InnerNode>(currentNode);

		if (currentInnerNode->inferiorChild->volume.contains(referencePoint)) {
			currentNode = currentInnerNode->inferiorChild;
		}
		else {
			currentNode = currentInnerNode->superiorChild;
		}
	}

	return std::static_pointer_cast<LeafNode>(currentNode);
}

void clear(std::stack<std::shared_ptr<Node>>& stack) {
	std::stack<std::shared_ptr<Node>> emptyStack;
	std::swap(stack, emptyStack);
}

std::shared_ptr<Node> topAndPop(std::stack<std::shared_ptr<Node>>& stack) {
	std::shared_ptr<Node> topElement = stack.top();
	stack.pop();
	return topElement;
}

std::shared_ptr<LeafNode> ThreeDTree::_findLeafNodeNearerTo(Point3d const & referencePoint, std::shared_ptr<LeafNode> givenNearLeafNode) const {

	std::shared_ptr<LeafNode> result = nullptr;
	std::stack<std::shared_ptr<Node>> nodesForVisit;

	double givenDistance = distance3d(referencePoint, *givenNearLeafNode->pointPtr);
	Interval3d intervalOfPossiblyNearerPoints = Interval3d::getSphereEncapsulatingIntervalFor(referencePoint, givenDistance);

	nodesForVisit.push(this->_root);

	while (!nodesForVisit.empty()) {
		std::shared_ptr<Node> currentNode = topAndPop(nodesForVisit);

		if (currentNode->isLeafNode()) {

			std::shared_ptr<LeafNode> currentLeafNode = std::static_pointer_cast<LeafNode>(currentNode);
			double currentDistance = distance3d(referencePoint, *currentLeafNode->pointPtr);
			if (currentDistance < givenDistance) {
				result = currentLeafNode;
				clear(nodesForVisit);
			}
		}
		else {

			std::shared_ptr<InnerNode> currentInnerNode = std::static_pointer_cast<InnerNode>(currentNode);
			if (currentInnerNode->superiorChild->volume.intersectsWith(intervalOfPossiblyNearerPoints)) {
				nodesForVisit.push(currentInnerNode->superiorChild);
			}
			if (currentInnerNode->inferiorChild->volume.intersectsWith(intervalOfPossiblyNearerPoints)) {
				nodesForVisit.push(currentInnerNode->inferiorChild);
			}
		}
	}

	return result;
}

std::shared_ptr<LeafNode> ThreeDTree::getLeafNodeNearestTo(Point3d const & referencePoint) const {

	std::shared_ptr<LeafNode> currentlyNearestLeafNode = this->estimateLeafNodeNearOf(referencePoint);

	bool foundNearerLeafNodeInCurrentIteration;

	do {
		std::shared_ptr<LeafNode> nearerLeafNode = this->_findLeafNodeNearerTo(referencePoint, currentlyNearestLeafNode);

		foundNearerLeafNodeInCurrentIteration = nearerLeafNode != nullptr;

		if (foundNearerLeafNodeInCurrentIteration) {
			currentlyNearestLeafNode = nearerLeafNode;
		}
	} while (foundNearerLeafNodeInCurrentIteration);

	return currentlyNearestLeafNode;
}

struct ProcessStepInformation {

	std::shared_ptr<Node> node;
	int level;

	ProcessStepInformation(std::shared_ptr<Node> node, int level)
		:node(node),
		level(level)
	{}
};

// int parameter is level or distance to root
// callback is for early abort after finishing functor


void ThreeDTree::_traversTreeAt(
	std::shared_ptr<Node> root,
	std::function<void(std::shared_ptr<LeafNode>, int, std::function<void()>)> handleLeafNode,
	std::function<void(std::shared_ptr<InnerNode>, int, std::function<void()>)> handleInnerNode,
	std::function<bool(InnerNode const &, int)> canExcludeInferiorChildOf,
	std::function<bool(InnerNode const &, int)> canExcludeSuperiorChildOf,
	std::function<bool(Node const &, int)> canExcludeRoot
) {

	std::stack<ProcessStepInformation> openProcessSteps;
	bool shouldAbort = false;
	std::function<void()> abort = [&shouldAbort]()->void {
		shouldAbort = true;
	};
	int const initialLevel = 0;

	if (canExcludeRoot(*root, initialLevel)) {
		// exclude whole tree, so there is nothing to do
	}
	else {
		openProcessSteps.push(ProcessStepInformation(root, initialLevel));
	}

	while (!openProcessSteps.empty() && !shouldAbort) {
		std::shared_ptr<Node> currentNode = openProcessSteps.top().node;
		int currentLevel = openProcessSteps.top().level;
		openProcessSteps.pop();

		if (currentNode->isLeafNode()) {

			std::shared_ptr<LeafNode> currentLeafNode = std::static_pointer_cast<LeafNode>(currentNode);
			handleLeafNode(currentLeafNode, currentLevel, abort);
		}
		else {

			std::shared_ptr<InnerNode> currentInnerNode = std::static_pointer_cast<InnerNode>(currentNode);
			handleInnerNode(currentInnerNode, currentLevel, abort);

			if (canExcludeSuperiorChildOf(*currentInnerNode, currentLevel)) {
				// exlude subtree of superior child, so there is nothing to do
			}
			else {
				openProcessSteps.push(ProcessStepInformation(currentInnerNode->superiorChild, currentLevel + 1));
			}

			if (canExcludeInferiorChildOf(*currentInnerNode, currentLevel)) {
				// exlude subtree of inferior child, so there is nothing to so
			}
			else {
				openProcessSteps.push(ProcessStepInformation(currentInnerNode->inferiorChild, currentLevel + 1));
			}
		}
	}
}

void ThreeDTree::hideSubtreeAt(std::shared_ptr<Node> root, bool shouldStopDescendingAtAlreadySetNodes) {

	std::function<void(std::shared_ptr<LeafNode>, int, std::function<void()>)> handleLeafNode
		= [](std::shared_ptr<LeafNode> node, int level, std::function<void()> abort)->void {
		node->hide();
	};

	std::function<void(std::shared_ptr<InnerNode>, int, std::function<void()>)> handleInnerNode
		= [](std::shared_ptr<InnerNode> node, int level, std::function<void()> abort)->void {
		node->hide();
	};

	std::function<bool(InnerNode const &, int)> canExcludeInferiorChildOf
		= [shouldStopDescendingAtAlreadySetNodes](InnerNode const & node, int level)->bool {
		if (shouldStopDescendingAtAlreadySetNodes && node.inferiorChild->isHidden()) {
			return true;
		}
		else {
			return false;
		}
	};

	std::function<bool(InnerNode const &, int)> canExcludeSuperiorChildOf
		= [shouldStopDescendingAtAlreadySetNodes](InnerNode const & node, int level)->bool {
		if (shouldStopDescendingAtAlreadySetNodes && node.superiorChild->isHidden()) {
			return true;
		}
		else {
			return false;
		}
	};

	ThreeDTree::_traversTreeAt(
		root,
		handleLeafNode,
		handleInnerNode,
		canExcludeInferiorChildOf,
		canExcludeSuperiorChildOf
	);
}
void ThreeDTree::unhideSubtreeAt(std::shared_ptr<Node> root, bool shouldStopDescendingAtAlreadySetNodes) {

	std::function<void(std::shared_ptr<LeafNode>, int, std::function<void()>)> handleLeafNode
		= [](std::shared_ptr<LeafNode> node, int level, std::function<void()> abort)->void {
		node->unhide();
	};

	std::function<void(std::shared_ptr<InnerNode>, int, std::function<void()>)> handleInnerNode
		= [](std::shared_ptr<InnerNode> node, int level, std::function<void()> abort)->void {
		node->unhide();
	};

	std::function<bool(InnerNode const &, int)> canExcludeInferiorChildOf
		= [shouldStopDescendingAtAlreadySetNodes](InnerNode const & node, int level)->bool {
		if (shouldStopDescendingAtAlreadySetNodes && !node.inferiorChild->isHidden()) {
			return true;
		}
		else {
			return false;
		}
	};

	std::function<bool(InnerNode const &, int)> canExcludeSuperiorChildOf
		= [shouldStopDescendingAtAlreadySetNodes](InnerNode const & node, int level)->bool {
		if (shouldStopDescendingAtAlreadySetNodes && !node.superiorChild->isHidden()) {
			return true;
		}
		else {
			return false;
		}
	};

	ThreeDTree::_traversTreeAt(
		root,
		handleLeafNode,
		handleInnerNode,
		canExcludeInferiorChildOf,
		canExcludeSuperiorChildOf
	);
}

void ThreeDTree::hideAllNodes() {
	ThreeDTree::hideSubtreeAt(this->_root);
}

void ThreeDTree::unhideAllNodes() {
	ThreeDTree::unhideSubtreeAt(this->_root);
}

void ThreeDTree::toEachLeafNodeApply(std::function<void(LeafNode &)> functor) {

	std::for_each(
		this->_leafNodePtrs->begin(), this->_leafNodePtrs->end(),
		[functor](std::shared_ptr<LeafNode> leafNodePtr)->void {
			functor(*leafNodePtr);
		}
	);
}

void ThreeDTree::toEachLeafNodeApply(std::function<void(LeafNode const &)> functor) const {

	std::for_each(
		this->_leafNodePtrs->begin(), this->_leafNodePtrs->end(),
		[functor](std::shared_ptr<LeafNode const> const leafNodePtr)->void {
			functor(*leafNodePtr);
		}
	);
}

size_t ThreeDTree::getNumberOfLeafNodes() const {
	size_t result = 0;

	if (this->_leafNodePtrs != nullptr) {
		result = this->_leafNodePtrs->size();
	}

	return result;
}
