#pragma once

#include "Interval3d.h"

struct Node {

	Interval3d volume;
	bool hidden;

	Node(const Interval3d& interval);
	virtual ~Node();
	virtual bool isLeafNode() const = 0;

	void hide();
	void unhide();
	bool isHidden();
};