#pragma once

#include "Node.h"
#include <memory>
#include "Bisector3d.h"

struct InnerNode : Node {

	std::shared_ptr<Node> inferiorChild;
	std::shared_ptr<Node> superiorChild;
	Bisector3d bisector;

	InnerNode(Interval3d const & volume = Interval3d::all(), std::shared_ptr<Node> inferiorChild = nullptr, std::shared_ptr<Node> superiorChild = nullptr, Bisector3d const & bisector = Bisector3d());
	virtual ~InnerNode();
	virtual bool isLeafNode() const override;
};