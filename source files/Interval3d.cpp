#include "Interval3d.h"

Interval3d::Interval3d(const Interval& x, const Interval& y, const Interval& z) 
	:x(x),
	y(y),
	z(z)
{
}

Interval3d Interval3d::all() {
	return Interval3d(
		Interval::all(),
		Interval::all(),
		Interval::all()
	);
}