#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <QColor>
#include "Point3d.h"
#include "ThreeDTree.h"
#include "RenderableObjects.h"

class PointCloud3d : public RenderableObjects
{
public:
	PointCloud3d(
		std::function<void(std::shared_ptr<std::vector<Point3d>>)> storeCreatedPointData,
		std::vector<Point3d*> const & pointData = std::vector<Point3d*>()
	);
	~PointCloud3d();

	void setPointDataTo(std::vector<Point3d*> const & pointData);	// TODO: needed in public?
	void setNormalDataTo(std::shared_ptr<std::vector<Point3d>> const & normalData);
	void setColor(QColor color);
	bool isEmpty() const;
	size_t getNumberOfPoints() const;
	void toEachPointApply(std::function<void(Point3d*)> functor) const;

	QColor getColor() const;
	std::shared_ptr<std::vector<Point3d>> getNormals();
	std::shared_ptr<std::vector<Point3d*>> query(Point3d const & referencePoint, double maximumDistance) const;
	std::shared_ptr<PointCloud3d> computeSmoothedVersionWith(double radius);
	std::shared_ptr<PointCloud3d> computeThinnedVersionWith(double thinningRadius);
	void recomputeTree(); // TODO: should not be necessary except when somebody manipulates pointData (what should not happen)

private:
	std::function<void(std::shared_ptr<std::vector<Point3d>>)> _storeCreatedPointData;
	std::shared_ptr<std::vector<Point3d>> _normalVectorsPtr;
	ThreeDTree _tree;
	std::pair<Point3d, Point3d> _minMax;
	QColor _color;

	virtual void _computeCenter() ;
	virtual void _computeRadius() ;
	void _computeBoundingBox();
};
