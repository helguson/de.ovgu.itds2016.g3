#include "Node.h"

Node::Node(const Interval3d& interval)
	:volume(interval),
	hidden(false)
{}

Node::~Node() {}

void Node::hide() {
	this->hidden = true;
}

void Node::unhide() {
	this->hidden = false;
}
bool Node::isHidden() {
	return this->hidden;
}