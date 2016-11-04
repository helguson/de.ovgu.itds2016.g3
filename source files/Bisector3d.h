#pragma once

#include "Dimension.h"
#include <tuple>
#include "Interval3d.h"
#include "LeafNode.h"
#include <memory>

struct Bisector3d {

	Dimension dimension;
	std::shared_ptr<LeafNode> medianElementPtr;

	Bisector3d(Dimension dimension = Dimension::UNDEFINED, std::shared_ptr<LeafNode> medianElementPtr = nullptr);
	std::tuple<Interval3d, Interval3d> bisect(const Interval3d& interval) const;
};