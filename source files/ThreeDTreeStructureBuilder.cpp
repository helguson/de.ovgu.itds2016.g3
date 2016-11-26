#include "ThreeDTreeStructureBuilder.h"

#include <algorithm>

#include <iostream>
#include <string>

#include "testHelpers.h"

void test01_manualStructure() {

	log("####################");
	log("### start test01 ###");
	log("####################");

	Point3d p1(1, 0, 0);
	Point3d p2(2, 0, 0);

	Interval3d i = Interval3d::all();

	std::shared_ptr<LeafNode> l1p = std::make_shared<LeafNode>();
	std::shared_ptr<LeafNode> l2p = std::make_shared<LeafNode>();

	l1p->pointPtr = &p1;
	l2p->pointPtr = &p2;

	std::shared_ptr<InnerNode> rootp = std::make_shared<InnerNode>(i);

	rootp->bisector.dimension = Dimension::X;
	rootp->bisector.medianElementPtr = l1p;

	rootp->inferiorChild = l1p;
	rootp->superiorChild = l2p;

	std::tuple<Interval3d, Interval3d> childIntervals = rootp->bisector.bisect(i);

	rootp->inferiorChild->volume = std::get<0>(childIntervals);
	rootp->superiorChild->volume = std::get<1>(childIntervals);

}

void test02_buildFor_1Point() {
	log("####################");
	log("### start test02 ###");
	log("####################");

	std::vector<Point3d> data;
	data.push_back(Point3d(1, 1, 1));

	ThreeDTreeStructureBuilder builder;
	std::shared_ptr<Node> root = builder.buildStructureFor(data.begin(), data.end()).root;

	log("one point tree root should be a leaf: " + toStr(root->isLeafNode()));
}

void test03_buildFor_2Points() {
	log("####################");
	log("### start test03 ###");
	log("####################");

	std::vector<Point3d> data;
	data.push_back(Point3d(1, 1, 1));
	data.push_back(Point3d(2, 2, 2));

	ThreeDTreeStructureBuilder builder;
	std::shared_ptr<Node> structureRootPtr = builder.buildStructureFor(data.begin(), data.end()).root;

	log("two point tree root should be no leaf: " + toStr(!structureRootPtr->isLeafNode()));

	if (!structureRootPtr->isLeafNode()) {
		std::shared_ptr<InnerNode> rootPtr = std::static_pointer_cast<InnerNode>(structureRootPtr);
		log("median point of root should be (1,1,1): " + rootPtr->bisector.medianElementPtr->pointPtr->toStr());
	}
}

void test04_buildFor_3Points() {
	log("####################");
	log("### start test04 ###");
	log("####################");

	std::vector<Point3d> data;
	data.push_back(Point3d(1, 1, 1));
	data.push_back(Point3d(2, 2, 2));
	data.push_back(Point3d(3, 3, 3));

	ThreeDTreeStructureBuilder builder;
	std::shared_ptr<Node> structureRootPtr = builder.buildStructureFor(data.begin(), data.end()).root;

	/* tree should look like:
	 *
	 *          root
	 *         (inner)
	 *        /       \
	 *       i2
	 *     (inner)   (leaf)
	 *      /    \
	 *    l1
	 *  (leaf)  (leaf)
	 *
	 * subdivision should be like:
	 *
	 *                  |
	 *                  |
	 *                  |  (3,3)-*
	 *            (2,2)-*
	 * ----------*--------------
	 *           |      |
	 *       (1,1)      |
	 *                  |
	 * So the most inferior leaf should be [-Inf,2][-Inf,1][-Inf,Inf]
	 */

	if (!structureRootPtr->isLeafNode()) {
		std::shared_ptr<InnerNode> rootPtr = std::static_pointer_cast<InnerNode>(structureRootPtr);
		std::shared_ptr<InnerNode> i2Ptr = std::static_pointer_cast<InnerNode>(rootPtr->inferiorChild);
		std::shared_ptr<LeafNode> l1Ptr = std::static_pointer_cast<LeafNode>(i2Ptr->inferiorChild);
		log("most inferior leaf should be [-Inf,2][-Inf,1][-Inf,Inf] and is: " + l1Ptr->volume.x.toString() + l1Ptr->volume.y.toString() + l1Ptr->volume.z.toString());
	}
}

void ThreeDTreeStructureBuilder::runTest() {

	test01_manualStructure();
	test02_buildFor_1Point();
	test03_buildFor_2Points();
	test04_buildFor_3Points();
}

ThreeDTreeStructureBuilder::ThreeDTreeStructureBuilder()
	:_leafPtrs(std::make_shared<std::vector<std::shared_ptr<LeafNode>>>()),
	_root(),
	_toProcessStorage()
{
}

ThreeDTreeStructureBuilder::BuildResults ThreeDTreeStructureBuilder::buildStructureFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd) {
	
	this->_initializeWith(dataBegin, dataEnd);

	while (this->_hasSomethingForProcessing()) {
		ProcessStepInformation processStep = this->_takeNextProcessStepInformation();

		if (processStep.getNumberOfElements() > 1) {
			this->_processToInnerNode(processStep);
		}
		else {
			// TODO: should be 1 element here
			this->_processToLeafNode(processStep);
		}
	}
		
	return this->_getResult();
}

void ThreeDTreeStructureBuilder::_createLeavesFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd) {

	size_t size = std::distance(dataBegin, dataEnd);
	this->_leafPtrs->reserve(size);

	std::vector<std::shared_ptr<LeafNode>>& leafNodePtrs = *(this->_leafPtrs);

	std::for_each(
		dataBegin, dataEnd,
		[&leafNodePtrs](Point3d& point)->void { 
		
			std::shared_ptr<LeafNode> leafPtr = std::make_shared<LeafNode>(Interval3d::all(), &point);

			leafNodePtrs.push_back(
				leafPtr
			);
		}
	);
}

bool ThreeDTreeStructureBuilder::_hasSomethingForProcessing() {
	return !this->_toProcessStorage.empty();
}

ThreeDTreeStructureBuilder::ProcessStepInformation ThreeDTreeStructureBuilder::_takeNextProcessStepInformation() {

	ProcessStepInformation result = this->_toProcessStorage.top();
	this->_toProcessStorage.pop();

	return result;
}

void ThreeDTreeStructureBuilder::_processToLeafNode(ThreeDTreeStructureBuilder::ProcessStepInformation information) {

	// connect
	std::shared_ptr<Node>& parentToChildPtr = information.parentToChildPtr;
	std::shared_ptr<LeafNode> leafNodePtr = *(information.rangeFirst);
	parentToChildPtr = leafNodePtr;

	// set interval
	leafNodePtr->volume = information.representedInterval;
}

void ThreeDTreeStructureBuilder::_processToInnerNode(ThreeDTreeStructureBuilder::ProcessStepInformation information) {

	MedianSplit medianSplit = this->_splitAccordingToMedian(
		information.rangeFirst, information.rangeLast, information.splitDimension
	);

	Bisector3d bisector(information.splitDimension, medianSplit.medianPtr);
	std::tuple<Interval3d, Interval3d> childIntervals = bisector.bisect(information.representedInterval);

	std::shared_ptr<InnerNode> nodePtr = std::make_shared<InnerNode>(
		information.representedInterval,
		nullptr, nullptr,		// inferior and superior child will be set later
		bisector
	);

	// add to current structure
	information.parentToChildPtr = nodePtr;

	// add inferior and superior ranges for processing
	ProcessStepInformation inferiorRangeProcessing(
		std::get<0>(childIntervals),
		nodePtr->inferiorChild,
		medianSplit.inferiorFirst, medianSplit.inferiorLast,
		this->_getDimensionForChildrenAccordingTo(information.splitDimension)
	);
	ProcessStepInformation superiorRangeProcessing(
		std::get<1>(childIntervals),
		nodePtr->superiorChild,
		medianSplit.superiorFirst, medianSplit.superiorLast,
		this->_getDimensionForChildrenAccordingTo(information.splitDimension)
	);
	this->_toProcessStorage.push(superiorRangeProcessing);
	this->_toProcessStorage.push(inferiorRangeProcessing);
}

void ThreeDTreeStructureBuilder::_initializeWith(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd) {

	this->_createLeavesFor(dataBegin, dataEnd);

	ProcessStepInformation information(Interval3d::all(), this->_root, this->_leafPtrs->begin(), this->_leafPtrs->end(), Dimension::X);
	this->_toProcessStorage.push(information);
}

ThreeDTreeStructureBuilder::BuildResults ThreeDTreeStructureBuilder::_getResult() {

	std::shared_ptr<Node> root = this->_root;
	this->_root = nullptr;

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> leafNodePtrs = this->_leafPtrs;
	this->_leafPtrs = std::make_shared<std::vector<std::shared_ptr<LeafNode>>>();

	return BuildResults(root, leafNodePtrs);
}

ThreeDTreeStructureBuilder::MedianSplit ThreeDTreeStructureBuilder::_splitAccordingToMedian(std::vector<std::shared_ptr<LeafNode>>::iterator first,
	std::vector<std::shared_ptr<LeafNode>>::iterator last, Dimension splitDimension) {

	size_t shiftToMedian = (std::distance(first, last)-1)/2;
	std::vector<std::shared_ptr<LeafNode>>::iterator medianElementIterator = first + shiftToMedian;

	std::nth_element(
		first, medianElementIterator, last,
		this->_getIsFirstInferiorFunctorAccordingTo(splitDimension)
	);

	return MedianSplit(
		first, medianElementIterator + 1,
		*medianElementIterator,
		medianElementIterator + 1, last
	);
}

std::function<bool(std::shared_ptr<LeafNode>, std::shared_ptr<LeafNode>)> ThreeDTreeStructureBuilder::_getIsFirstInferiorFunctorAccordingTo(Dimension splitDimension) {
	
	std::function<double(std::shared_ptr<LeafNode>)> valueAccessor;

	switch (splitDimension)
	{
	case Dimension::X:
		valueAccessor = [](std::shared_ptr<LeafNode> leafNodePtr)->double { return leafNodePtr->pointPtr->x;  };
		break;
	case Dimension::Y:
		valueAccessor = [](std::shared_ptr<LeafNode> leafNodePtr)->double { return leafNodePtr->pointPtr->y;  };
		break;
	case Dimension::Z:
		valueAccessor = [](std::shared_ptr<LeafNode> leafNodePtr)->double { return leafNodePtr->pointPtr->z;  };
		break;
	default:
		// TODO: should not happen
		break;
	}

	return [valueAccessor](std::shared_ptr<LeafNode> first, std::shared_ptr<LeafNode> second)->bool {
		return valueAccessor(first) < valueAccessor(second);
	};
}

Dimension ThreeDTreeStructureBuilder::_getDimensionForChildrenAccordingTo(Dimension currentDimension) {
	Dimension result = Dimension::UNDEFINED;

	switch (currentDimension)
	{
	case Dimension::X:
		result = Dimension::Y;
		break;
	case Dimension::Y:
		result = Dimension::Z;
		break;
	case Dimension::Z:
		result = Dimension::X;
		break;
	default:
		// TODO: should not happen
		break;
	}

	return result;
}