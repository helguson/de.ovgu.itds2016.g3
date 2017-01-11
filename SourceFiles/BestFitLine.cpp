#include "BestFitLine.h"

BestFitLine::BestFitLine(PointCloud3d const & referencePointCloud)
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

	this->_computeDefiningVectorsAccordingTo(points);
	this->_computeRepresentativePointsAccordingTo(points);
	this->_computeCenter();
	this->_computeRadius();
}

BestFitLine::~BestFitLine(){}

double BestFitLine::computeDistanceTo(Point3d const & point) const {
	return distancePt2Line(point, this->_positionVector, this->_directionVector);
}

std::vector<Point3d> const & BestFitLine::getRepresentativePoints() const {
	return this->_representativePoints;
}

void BestFitLine::_computeDefiningVectorsAccordingTo(std::vector<Point3d> const & points) {

	this->_positionVector = computeCenter(points);

	Matrix M(3, 3);
	computeCovarianceMatrix3x3(points, M);	// M is covariance matrix
	SVD::computeSymmetricEigenvectors(M);	// M is matrix of eigenvectors

	const Point3d ev0(M(0, 0), M(1, 0), M(2, 0)); //first column of M == Eigenvector corresponding to the largest Eigenvalue == direction of biggest variance
	const Point3d ev1(M(0, 1), M(1, 1), M(2, 1));
	const Point3d ev2(M(0, 2), M(1, 2), M(2, 2)); //third column of M == Eigenvector corresponding to the smallest Eigenvalue == direction of lowest variance

	this->_directionVector = ev0;
}

// assume position vector and direction vector are already computed
void BestFitLine::_computeRepresentativePointsAccordingTo(std::vector<Point3d> const & points) {

	computeBestFitLine(points, this->_representativePoints, this->_positionVector, this->_directionVector);
}

void BestFitLine::_computeRadius()
{
	this->_radius = distance3d(this->_representativePoints[0], this->_representativePoints[1] ) / 2;
}

void BestFitLine::_computeCenter()
{
	this->_center = this->_positionVector;
}
