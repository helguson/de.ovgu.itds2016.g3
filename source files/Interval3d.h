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

	static Interval3d all();
};