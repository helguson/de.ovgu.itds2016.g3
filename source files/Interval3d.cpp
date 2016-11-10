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

bool Interval3d::intersectsWith(Interval3d const & otherInterval) const {

	return this->x.intersectsWith(otherInterval.x)
		&& this->y.intersectsWith(otherInterval.y)
		&& this->z.intersectsWith(otherInterval.z);
}

bool Interval3d::contains(Interval3d const & otherInterval) const {

	return this->x.contains(otherInterval.x)
		&& this->y.contains(otherInterval.y)
		&& this->z.contains(otherInterval.z);
}

bool Interval3d::contains(Point3d const & point) const {

	return this->x.contains(point.x)
		&& this->y.contains(point.y)
		&& this->z.contains(point.z);
}