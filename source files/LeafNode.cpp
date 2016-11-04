#include "LeafNode.h"


LeafNode::LeafNode(Interval3d const & volume, Point3d* pointPtr)
	:Node(volume),
	pointPtr(pointPtr)
{
}

LeafNode::~LeafNode(){}

bool LeafNode::isLeafNode() const {
	return true;
}