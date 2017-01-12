#include "BestFitPlane.h"
#include "Algorithms.h"
#include "SVD.h"

BestFitPlane::BestFitPlane(PointCloud3d const & referencePointCloud):
	RenderableObjects()
{
	std::vector<Point3d> points;
	points.reserve(referencePointCloud.getNumberOfPoints());

	// access point
	referencePointCloud.toEachPointApply(
		[&points](Point3d* pointPtr)->void {
		points.push_back(*pointPtr);
	}
	);

	this->_computeDefiningVectorsAccordingTo(points);
	this->_computeRepresentativePointsAccordingTo(points);
}

double BestFitPlane::computeDistanceTo(Point3d const & point) const
{
	return distancePt2Plane(point, this->_positionVector, this->_normalVector);
}

std::vector<Point3d> const & BestFitPlane::getRepresentativePoints() const
{
	return this->_representativePoints;
}

void BestFitPlane::_computeDefiningVectorsAccordingTo(std::vector<Point3d> const & points)
{
	this->_positionVector = computeCenter(points);

	Matrix M(3, 3);
	computeCovarianceMatrix3x3(points, M);	// M is covariance matrix
	SVD::computeSymmetricEigenvectors(M);	// M is matrix of eigenvectors

	const Point3d ev0(M(0, 0), M(1, 0), M(2, 0)); //first column of M == Eigenvector corresponding to the largest Eigenvalue == direction of biggest variance
	const Point3d ev1(M(0, 1), M(1, 1), M(2, 1));
	const Point3d ev2(M(0, 2), M(1, 2), M(2, 2)); //third column of M == Eigenvector corresponding to the smallest Eigenvalue == direction of lowest variance

	this->_normalVector = ev2;
	this->_normalVector = this->_normalVector * (1/vectorLength(this->_normalVector));
}

void BestFitPlane::_computeRepresentativePointsAccordingTo(std::vector<Point3d> const & points) 
{
	computeBestFitPlane(points, this->_representativePoints);
}

void BestFitPlane::_computeRadius()
{
	for each(Point3d corner in this->_representativePoints) {
		double distanceToCorner = distance3d(this->_center, corner);
		if (this->_radius < distanceToCorner)
			this->_radius = distanceToCorner;
	}
}

void BestFitPlane::_computeCenter()
{
	this->_center = this->_positionVector;
}
