#include "BestFitSphere.h"
#include "Matrix.h"

BestFitSphere::BestFitSphere(PointCloud3d const & referencePointCloud, size_t numberOfRepresentingStripes, size_t numberOfRepresentingRingElements)
	:	RenderableObjects()
{

	std::vector<Point3d> points;
	points.reserve(referencePointCloud.getNumberOfPoints());

	// access point
	referencePointCloud.toEachPointApply(
		[&points](Point3d* pointPtr)->void {
			points.push_back(*pointPtr);
		}
	);

	this->_computeDefiningAttributesAccordingTo(points, referencePointCloud.getCenter(), referencePointCloud.getRadius());
	this->_computeRepresentativePointsAccordingTo(numberOfRepresentingStripes, numberOfRepresentingRingElements);
	this->_computeCenter();
	this->_computeRadius();
}

BestFitSphere::~BestFitSphere(){}

double BestFitSphere::computeDistanceTo(Point3d const & point) const {
	return distance3d(point, this->_center) - this->_radius;
}

std::vector<Point3d> const & BestFitSphere::getRepresentativePoints() const {
	return this->_representativePoints;
}

Matrix calculateDFrom(std::vector<Point3d> const & points, Matrix const & p)
{
	size_t m = points.size();
	Matrix result(m, 1);

	Point3d center(p[0], p[1], p[2]);
	double radius = p[3];

	for (size_t i = 0; i < m; i++) {
		result[i] = distance3d(points[i], center) - radius;
	}

	return result;
}

Matrix calculateJFrom(std::vector<Point3d> const & points, Matrix const & p) 
{
	size_t m = points.size();
	Matrix result(m, 4);

	Point3d center(p[0], p[1], p[2]);
	double radius = p[3];

	for (size_t i = 0; i < m; i++) {
		Point3d const & point = points[i];

		double distance = distance3d(point, center);
		result(i, 0) = -(point.x - center.x) / distance;
		result(i, 1) = -(point.y - center.y) / distance;
		result(i, 2) = -(point.z - center.z) / distance;
		result(i, 3) = -1;
	}

	return result;
}

void BestFitSphere::_computeDefiningAttributesAccordingTo(std::vector<Point3d> const & points, Point3d center, double radius) {

	// set initial guess
	Matrix p(4, 1);	// parameter vector
	p[0] = center.x;
	p[1] = center.y;
	p[2] = center.z;
	p[3] = radius;

	// descend along gradient
	size_t const MAXIMUM_NUMBER_OF_ITERATIONS = 10;
	size_t numberOfFinishedIterations = 0;

	while (numberOfFinishedIterations < MAXIMUM_NUMBER_OF_ITERATIONS) {

		// J*dp = -d
		Matrix J = calculateJFrom(points, p);	// Jakobi matrix
		Matrix d = calculateDFrom(points, p);	// error vector
		
		Matrix dp = J.solveFor(-1.0*d);
		p = p + dp;

		numberOfFinishedIterations++;
	}

	// set parameters
	this->_center.x = p[0];
	this->_center.y = p[1];
	this->_center.z = p[2];
	this->_radius = p[3];
}

// assume that center and radius are computed
void BestFitSphere::_computeRepresentativePointsAccordingTo(size_t numberOfRepresentingStripes, size_t numberOfRepresentingRingElements) {

	double const PI = 3.14159;
	double const dAlpha = PI / numberOfRepresentingStripes;
	double const dBeta = 2 * PI / numberOfRepresentingRingElements;

	for (size_t iStripe = 0; iStripe < numberOfRepresentingStripes; iStripe++) {
		double alpha = iStripe*dAlpha;
		double radius1 = sin(alpha)*this->_radius;
		double radius2 = sin(alpha+dAlpha)*this->_radius;
		double z1 = cos(alpha)*this->_radius;
		double z2 = cos(alpha + dAlpha)*this->_radius;

		for (size_t iPatch = 0; iPatch < numberOfRepresentingRingElements; iPatch++) {
			double beta = iPatch*dBeta;
			
			// 1---4
			// | / |
			// 2---3

			Point3d p1(radius1*cos(beta), radius1*sin(beta), z1);
			Point3d p2(radius2*cos(beta), radius2*sin(beta), z2);
			Point3d p3(radius2*cos(beta+dBeta), radius2*sin(beta+dBeta), z2);
			Point3d p4(radius1*cos(beta+dBeta), radius1*sin(beta+dBeta), z1);

			// 1---4
			// | /
			// 2

			this->_representativePoints.push_back(this->_center + p1);
			this->_representativePoints.push_back(this->_center + p2);
			this->_representativePoints.push_back(this->_center + p4);

			//     4
			//   / |
			// 2---3
			this->_representativePoints.push_back(this->_center + p2);
			this->_representativePoints.push_back(this->_center + p3);
			this->_representativePoints.push_back(this->_center + p4);
		}
	}
}

// 
void BestFitSphere::_computeRadius()
{
	// nothing to do, task is done in _computeDefiningAttributesAccordingTo
	// TODO: remove?
}

void BestFitSphere::_computeCenter()
{
	// nothing to do, task is done in _computeDefiningAttributesAccordingTo
	// TODO: remove?
}
