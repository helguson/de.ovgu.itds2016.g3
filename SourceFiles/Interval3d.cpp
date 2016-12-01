#include "Interval3d.h"
#include <algorithm>

#include "testHelpers.h"

void test01_contains_interval3d() {
	log("####################");
	log("### start test01 ###");
	log("####################");

	Interval3d i1 = Interval3d::all();
	Interval3d i2(
		Interval::all(),
		Interval(-1, true, 1, true), // [-1,1]
		Interval::all()
	);

	log("all should contain all: " + toStr(i1.contains(i1)));
	log("all should contain almost all: " + toStr(i1.contains(i2)));
}

void Interval3d::runTest() {
	test01_contains_interval3d();
}


Interval3d::Interval3d(const Interval& x, const Interval& y, const Interval& z) 
	:x(x),
	y(y),
	z(z)
{}

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

Interval3d Interval3d::getSphereEncapsulatingIntervalFor(Point3d const & centerPoint, double radius) {

	return Interval3d(
		Interval(centerPoint.x - radius, true, centerPoint.x + radius, true),
		Interval(centerPoint.y - radius, true, centerPoint.y + radius, true),
		Interval(centerPoint.z - radius, true, centerPoint.z + radius, true)
	);

}

std::vector<Point3d> Interval3d::getVertices() const {
	std::vector<Point3d> vertices;

	double xMin = this->x.inferiorBorder;
	double xMax = this->x.superiorBorder;

	double yMin = this->y.inferiorBorder;
	double yMax = this->y.superiorBorder;

	double zMin = this->z.inferiorBorder;
	double zMax = this->z.superiorBorder;

	vertices.push_back(Point3d(xMin, yMin, zMin));
	vertices.push_back(Point3d(xMax, yMin, zMin));
	vertices.push_back(Point3d(xMax, yMin, zMax));
	vertices.push_back(Point3d(xMin, yMin, zMax));

	vertices.push_back(Point3d(xMin, yMax, zMin));
	vertices.push_back(Point3d(xMax, yMax, zMin));
	vertices.push_back(Point3d(xMax, yMax, zMax));
	vertices.push_back(Point3d(xMin, yMax, zMax));

	return vertices;
}

bool Interval3d::isContainedInSphereWith(Point3d const & center, double radius) const {

	bool result = true;

	std::vector<Point3d> verticesOfInterval = this->getVertices();

	std::for_each(
		verticesOfInterval.begin(), verticesOfInterval.end(),
		[&result, center, radius](Point3d const & vertex)->void {
			result = result && (distance3d(vertex, center) <= radius);
		}
	);

	return result;
}