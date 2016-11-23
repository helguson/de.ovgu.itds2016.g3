#pragma once

#include "Interval3d.h"

struct Node {

	Interval3d volume;

	Node(const Interval3d& interval);
	virtual ~Node();
	virtual bool isLeafNode() const = 0;
};