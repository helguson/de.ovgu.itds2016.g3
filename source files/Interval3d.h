#pragma once

#include "Interval.h"
#include "Point3d.h"

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

	static Interval3d all();
};