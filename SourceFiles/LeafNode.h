#pragma once

#include "Node.h"
#include "Point3d.h"

struct LeafNode: Node
{
	Point3d* pointPtr;

	LeafNode(Interval3d const & volume = Interval3d::all(), Point3d* pointPtr = nullptr);
	virtual ~LeafNode();
	virtual bool isLeafNode() const override;
};