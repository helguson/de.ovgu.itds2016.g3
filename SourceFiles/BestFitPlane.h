#pragma once

#include "PointCloud3d.h"
#include "Point3d.h"
#include "RenderableObjects.h"
#include <vector>

class BestFitPlane : public RenderableObjects
{
public:
	BestFitPlane(PointCloud3d const & referencePointCloud);

	double computeDistanceTo(Point3d const & point) const;
	std::vector<Point3d> const & getRepresentativePoints() const;

private:
	Point3d _positionVector;
	Point3d _normalVector;
	std::vector<Point3d> _representativePoints;

	void _computeDefiningVectorsAccordingTo(std::vector<Point3d> const & points);
	void _computeRepresentativePointsAccordingTo(std::vector<Point3d> const & points);

	virtual void _computeRadius();
	virtual void _computeCenter();
};

