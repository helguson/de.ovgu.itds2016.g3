#include "ThreeDTree.h"
#include <algorithm>
#include "LeafNode.h"
#include "ThreeDTreeStructureBuilder.h"

void ThreeDTree::buildFor(std::vector<Point3d>::iterator dataBegin, std::vector<Point3d>::iterator dataEnd) {

	size_t size = std::distance(dataBegin, dataEnd);

	if (size >= 1) {
		ThreeDTreeStructureBuilder builder;
		this->_root = builder.buildStructureFor(dataBegin, dataEnd);
	}
	else {
		this->_root = nullptr;
	}
}

