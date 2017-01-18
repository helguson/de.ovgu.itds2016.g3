#pragma once
#include "Algorithms.h"
#include "SVD.h"
#include "RenderableObjects.h"
#include "PointCloud3d.h"
#include "Point3d.h"
#include <vector>

class BestFitSphere : public RenderableObjects
{
public:
	BestFitSphere(PointCloud3d const & referencePointCloud, size_t numberOfRepresentingStripes = 8, size_t numberOfRepresentingRingElements = 16);
	~BestFitSphere();
	double computeDistanceTo(Point3d const & point) const;
	std::vector<Point3d> const & getRepresentativePoints() const;

private:
	std::vector<Point3d> _representativePoints;

	void _computeDefiningAttributesAccordingTo(std::vector<Point3d> const & points, Point3d center, double radius);
	void _computeRepresentativePointsAccordingTo(size_t numberOfRepresentingStripes, size_t numberOfRepresentingRingElements);

	virtual void _computeRadius();
	virtual void _computeCenter();
};