#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include "Point3d.h"
#include "ThreeDTree.h"

class PointCloud3d
{
public:
	PointCloud3d(
		std::function<void(std::shared_ptr<std::vector<Point3d>>)> storeCreatedPointData,
		std::vector<Point3d*> const & pointData = std::vector<Point3d*>()
	);
	~PointCloud3d();

	void setPointDataTo(std::vector<Point3d*> const & pointData);	// TODO: needed in public?
	bool isEmpty() const;
	size_t getNumberOfPoints() const;
	void toEachPointApply(std::function<void(Point3d*)> functor) const;

	Point3d getCenter() const;
	double getRadius() const;

	std::shared_ptr<std::vector<Point3d*>> query(Point3d const & referencePoint, double maximumDistance) const;
	std::shared_ptr<PointCloud3d> computeSmoothedVersionWith(double radius);
	std::shared_ptr<PointCloud3d> computeThinnedVersionWith(double thinningRadius);
	void recomputeTree(); // TODO: should not be necessary except when somebody manipulates pointData (what should not happen)

private:
	std::function<void(std::shared_ptr<std::vector<Point3d>>)> _storeCreatedPointData;
	ThreeDTree _tree;
	std::pair<Point3d, Point3d> _minMax;
	Point3d _sceneCenter;
	double _sceneRadius;

	void _computeCenter();
	void _computeRadius();
	void _computeBoundingBox();
};
