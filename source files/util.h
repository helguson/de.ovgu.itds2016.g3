#pragma once

#include <memory>
#include <vector>
#include "Point3d.h"

namespace util {

	std::shared_ptr<std::vector<Point3d*>> computePointPtrVectorFrom(std::vector<Point3d> & pointData);
}