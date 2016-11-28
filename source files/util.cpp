#include "util.h"

#include <algorithm>

std::shared_ptr<std::vector<Point3d*>> util::computePointPtrVectorFrom(std::vector<Point3d> & pointData) {

	std::shared_ptr<std::vector<Point3d*>> result = std::make_shared<std::vector<Point3d*>>();
	result->reserve(pointData.size());

	std::for_each(
		pointData.begin(), pointData.end(),
		[&result](Point3d & point) {
			result->push_back(&point);
		}
	);

	return result;
}