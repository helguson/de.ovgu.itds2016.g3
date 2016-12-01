#pragma once

#include "Interval.h"
#include "Point3d.h"
#include <vector>

struct Interval3d
{
public:
	Interval x;
	Interval y;
	Interval z;
	
	Interval3d(const Interval& x = Interval::all(), const Interval& y = Interval::all(), const Interval& z = Interval::all());

	bool intersectsWith(Interval3d const & otherInterval) const;
	bool contains(Interval3d const & otherInterval) const;
	bool contains(Point3d const & point) const;
	bool isContainedInSphereWith(Point3d const & center, double radius) const;
	std::vector<Point3d> getVertices() const;

	static Interval3d all();
	static Interval3d getSphereEncapsulatingIntervalFor(Point3d const & centerPoint, double radius);
	static void runTest();

};