#pragma once

#include <vector>
#include <memory>
#include "Point3d.h"
#include "Node.h"
#include "InnerNode.h"
#include "LeafNode.h"
#include <stack>
#include "Dimension.h"
#include "Interval3d.h"
#include <iterator>
#include <functional>

class ThreeDTreeStructureBuilder
{
public:
	ThreeDTreeStructureBuilder();

	struct BuildResults {
		std::shared_ptr<Node> root;
		std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> leafNodePtrs;

		BuildResults(std::shared_ptr<Node> root, std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> leafNodePtrs) 
			:root(root),
			leafNodePtrs(leafNodePtrs)
		{}
	};

	// returns root of tree structure as well as a vector of all leaf nodes
	BuildResults buildStructureFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd);

	static void runTest();

private:
	struct ProcessStepInformation {
		Interval3d representedInterval;
		std::shared_ptr<Node>& parentToChildPtr;
		std::vector<std::shared_ptr<LeafNode>>::iterator rangeFirst;
		std::vector<std::shared_ptr<LeafNode>>::iterator rangeLast;
		Dimension splitDimension;

		ProcessStepInformation(Interval3d representedInterval, std::shared_ptr<Node>& parentToChildPtr,
			std::vector<std::shared_ptr<LeafNode>>::iterator rangeFirst,
			std::vector<std::shared_ptr<LeafNode>>::iterator rangeLast,
			Dimension splitDimension)
			:representedInterval(representedInterval),
			parentToChildPtr(parentToChildPtr),		//TODO: reference stored as attribute, is it possible?
			rangeFirst(rangeFirst),
			rangeLast(rangeLast),
			splitDimension(splitDimension)
		{}

		size_t getNumberOfElements() {
			return this->rangeLast - this->rangeFirst;
		}
	};

	struct MedianSplit {

		std::vector<std::shared_ptr<LeafNode>>::iterator inferiorFirst;
		std::vector<std::shared_ptr<LeafNode>>::iterator inferiorLast;
		std::shared_ptr<LeafNode> medianPtr;
		std::vector<std::shared_ptr<LeafNode>>::iterator superiorFirst;
		std::vector<std::shared_ptr<LeafNode>>::iterator superiorLast;

		MedianSplit(std::vector<std::shared_ptr<LeafNode>>::iterator inferiorFirst,
			std::vector<std::shared_ptr<LeafNode>>::iterator inferiorLast,
			std::shared_ptr<LeafNode> medianPtr,
			std::vector<std::shared_ptr<LeafNode>>::iterator superiorFirst,
			std::vector<std::shared_ptr<LeafNode>>::iterator superiorLast)
			:inferiorFirst(inferiorFirst),
			inferiorLast(inferiorLast),
			medianPtr(medianPtr),
			superiorFirst(superiorFirst),
			superiorLast(superiorLast)
		{}
	};

	std::shared_ptr<Node> _root;
	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> _leafPtrs;
	std::stack<ProcessStepInformation> _toProcessStorage; // TODO: match type

	void _createLeavesFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd);
	bool _hasSomethingForProcessing();
	void _processToLeafNode(ProcessStepInformation information);
	void _processToInnerNode(ProcessStepInformation information);
	ProcessStepInformation _takeNextProcessStepInformation();
	void _initializeWith(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd);
	BuildResults _getResult();
	/* 
	 * splits the given range [first, last) into an inferior range and an superior
	 * range according to the median element and a given dimension.
	 * The inferior range may contain elements all that are inferior as well some
	 * elments that are equivalent as well as the median element. The superior 
	 * range may contain elements that are superior to the median element as well
	 * as some elements that are equivalent.
	 * This method manipulates the given range.
	 */
	MedianSplit _splitAccordingToMedian(std::vector<std::shared_ptr<LeafNode>>::iterator first,
		std::vector<std::shared_ptr<LeafNode>>::iterator last, Dimension splitDimension);
	std::function<bool(std::shared_ptr<LeafNode>, std::shared_ptr<LeafNode>)> _getIsFirstInferiorFunctorAccordingTo(Dimension splitDimension);
	Dimension _getDimensionForChildrenAccordingTo(Dimension currentDimension);
};
