#pragma once

#include "Node.h"
#include <memory>
#include <vector>
#include "Point3d.h"
#include "LeafNode.h"
#include "InnerNode.h"
#include "functional"

class ThreeDTree {
public:

	struct QueryResult {
		std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> leafNodePtrs;
		std::shared_ptr<std::vector<std::shared_ptr<InnerNode>>> innerNodePtrs;

		QueryResult(
			std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> leafNodePtrs,
			std::shared_ptr<std::vector<std::shared_ptr<InnerNode>>> innerNodePtrs
		)
			:leafNodePtrs(leafNodePtrs),
			innerNodePtrs(innerNodePtrs)
		{}
	};

	ThreeDTree();
	void buildFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd);
	// TODO: add hidden handling
	QueryResult query(Interval3d const & range);
	QueryResult query(Point3d const & referencePoint, double maximumDistance);
	std::shared_ptr<LeafNode> estimateLeafNodeNearOf(Point3d const & referencePoint) const;
	// returns first leaf node with a point that has the minimal distance
	std::shared_ptr<LeafNode> getLeafNodeNearestTo(Point3d const & referencePoint) const;
	// TODO: need std::shared_ptr version?
	void toEachLeafNodeApply(std::function<void(LeafNode &)> functor);
	void hideAllNodes();
	void unhideAllNodes();

	static void hideSubtreeAt(std::shared_ptr<Node> root, bool shouldStopDescendingAtAlreadySetNodes = false);
	static void unhideSubtreeAt(std::shared_ptr<Node> root, bool shouldStopDescendingAtAlreadySetNodes = false);
	static void runTests();

private:
	std::shared_ptr<Node> _root;
	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> _leafNodePtrs;

	// returns nullPtr if no nearer leafNode was found
	std::shared_ptr<LeafNode> _findLeafNodeNearerTo(Point3d const & referencePoint, std::shared_ptr<LeafNode> currentNearLeafNode) const;
	static void _traversTreeAt(
		std::shared_ptr<Node> root, 
		std::function<void(std::shared_ptr<LeafNode>, int, std::function<void()>)> handleLeafNode
		= [](std::shared_ptr<LeafNode> node, int level, std::function<void()> abort)->void { /* nothing to do */ },
		std::function<void(std::shared_ptr<InnerNode>, int, std::function<void()>)> handleInnerNode
		= [](std::shared_ptr<InnerNode> node, int level, std::function<void()> abort)->void { /* nothing to do */ },
		std::function<bool(InnerNode const &, int)> canExcludeInferiorChildOf
		= [](InnerNode const & node, int level)->bool { return false; },
		std::function<bool(InnerNode const &, int)> canExcludeSuperiorChildOf
		= [](InnerNode const & node, int level)->bool { return false; },
		std::function<bool(Node const &, int)> canExcludeRoot
		= [](Node const & node, int level)->bool { return false; }
	);
};