#include "InnerNode.h"



InnerNode::InnerNode(Interval3d const & volume, std::shared_ptr<Node> inferiorChild, std::shared_ptr<Node> superiorChild, Bisector3d const & bisector)
	:Node(volume),
	inferiorChild(inferiorChild),
	superiorChild(superiorChild),
	bisector(bisector)
{
}

InnerNode::~InnerNode(){}

bool InnerNode::isLeafNode() const {
	return false;
}