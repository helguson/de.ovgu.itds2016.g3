#include "PointCloud3d.h"

#include "util.h"

PointCloud3d::PointCloud3d(
	std::function<void(std::shared_ptr<std::vector<Point3d>>)> storeCreatedPointData,
	std::vector<Point3d*> const & pointData
)
	:_storeCreatedPointData(storeCreatedPointData),
	_tree(pointData),
	_sceneCenter(),
	_sceneRadius(),
	_minMax()
{

	this->_computeBoundingBox();
	this->_computeCenter();
	this->_computeRadius();
}

PointCloud3d::~PointCloud3d() {}

void PointCloud3d::setPointDataTo(std::vector<Point3d*> const & pointData) {
	
	this->_tree.buildFor(pointData);

	this->_computeBoundingBox();
	this->_computeCenter();
	this->_computeRadius();
}

Point3d PointCloud3d::getCenter() const {

	return this->_sceneCenter;
}

double PointCloud3d::getRadius() const {

	return this->_sceneRadius;
}

void PointCloud3d::_computeCenter() {
		
	// compute center of scene
	this->_sceneCenter = (this->_minMax.first + this->_minMax.second) * 0.5;
}

void PointCloud3d::_computeRadius()
{
	this->_sceneRadius = distance3d(_minMax.second, this->_sceneCenter);
}

void PointCloud3d::_computeBoundingBox()
{
	Point3d min = Point3d(
		std::numeric_limits<double>::infinity(),
		std::numeric_limits<double>::infinity(),
		std::numeric_limits<double>::infinity()
	);
	Point3d max = Point3d(
		-std::numeric_limits<double>::infinity(),
		-std::numeric_limits<double>::infinity(),
		-std::numeric_limits<double>::infinity()
	);

	this->toEachPointApply(
		[&min, &max](Point3d* pointPtr)->void {

			const Point3d& point = *pointPtr;
			if (point.x < min.x) min.x = point.x;
			if (point.y < min.y) min.y = point.y;
			if (point.z < min.z) min.z = point.z;
			if (point.x > max.x) max.x = point.x;
			if (point.y > max.y) max.y = point.y;
			if (point.z > max.z) max.z = point.z;
		}
	);

	this->_minMax = std::pair<Point3d, Point3d>(min,max);
}

void PointCloud3d::recomputeTree() {
	this->_tree.rebuildStructure();
}

std::shared_ptr<std::vector<Point3d*>> PointCloud3d::query(Point3d const & referencePoint, double maximumDistance) const {

	std::shared_ptr<std::vector<std::shared_ptr<LeafNode>>> leafNodePtrs
		=  this->_tree.query(referencePoint,maximumDistance).leafNodePtrs;

	std::shared_ptr<std::vector<Point3d*>> result = std::make_shared<std::vector<Point3d*>>();
	result->reserve(leafNodePtrs->size());

	std::for_each(
		leafNodePtrs->begin(), leafNodePtrs->end(),
		[&result](std::shared_ptr<LeafNode> nodePtr)->void {
			result->push_back(nodePtr->pointPtr);
		}
	);

	return result;
}

bool PointCloud3d::isEmpty() const {
	return this->getNumberOfPoints() == 0;
}

size_t PointCloud3d::getNumberOfPoints() const {
	return this->_tree.getNumberOfLeafNodes();
}

void PointCloud3d::toEachPointApply(std::function<void(Point3d*)> functor) const {

	this->_tree.toEachLeafNodeApply(
		[functor](LeafNode const & leafNode)->void {
			functor(leafNode.pointPtr);
		}
	);
}

std::shared_ptr<PointCloud3d> PointCloud3d::computeSmoothedVersionWith(double radius)
{
	std::shared_ptr<std::vector<Point3d>> smoothedPoints 
		= std::make_shared<std::vector<Point3d>>();
	smoothedPoints->reserve(this->getNumberOfPoints());

	ThreeDTree const & tree = this->_tree;

	// TODO: #pragma omp parallel for
	this->toEachPointApply(
		[&smoothedPoints, this, radius](Point3d* pointPtr) {

			Point3d const & origin = *pointPtr;
			Point3d smoothedPt = Point3d(0, 0, 0);
			double weights = 0.0;
			std::shared_ptr<std::vector<Point3d*>> neighborhood = this->query(origin, radius);
			for each (Point3d* neighborPtr in *neighborhood)
			{
				Point3d const & neighbor = *neighborPtr;
				double wi = std::exp((-1.0 * distance3d(origin, neighbor)) / radius);
				smoothedPt += neighbor*wi;
				weights += wi;
			}
			smoothedPt *= (1.0 / weights);
			smoothedPoints->push_back(smoothedPt);
		}
	);

	this->_storeCreatedPointData(smoothedPoints);
	std::shared_ptr<std::vector<Point3d*>> smoothedPointPtrs = util::computePointPtrVectorFrom(*smoothedPoints);

	return std::make_shared<PointCloud3d>(this->_storeCreatedPointData, *smoothedPointPtrs);
}
