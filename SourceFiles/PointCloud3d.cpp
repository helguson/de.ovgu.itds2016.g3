#include "PointCloud3d.h"
#include "util.h"

PointCloud3d::PointCloud3d(
	std::function<void(std::shared_ptr<std::vector<Point3d>>)> storeCreatedPointData,
	std::vector<Point3d*> const & pointData
)
	:RenderableObjects(),
	_storeCreatedPointData(storeCreatedPointData),
	_normalVectorsPtr(std::make_shared<std::vector<Point3d>>()),
	_tree(pointData),
	_minMax(),
	_color(255,255,255)
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

void PointCloud3d::setNormalDataTo(std::shared_ptr<std::vector<Point3d>> const & normalData)
{
	this->_normalVectorsPtr = normalData;
}

void PointCloud3d::setColor(QColor color)
{
	this->_color = color;
}

QColor PointCloud3d::getColor() const
{
	return this->_color;
}

std::shared_ptr<std::vector<Point3d>> PointCloud3d::getNormals()
{
	return this->_normalVectorsPtr;
}

void PointCloud3d::_computeCenter() 
{
		
	// compute center of scene
	
	this->_center = (this->_minMax.first + this->_minMax.second) * 0.5;
}

void PointCloud3d::_computeRadius() 
{
	this->_radius = distance3d(_minMax.second, this->_center);
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

	
	this->toEachPointApply(
		[&smoothedPoints, this, radius](Point3d* pointPtr) {

			Point3d const & origin = *pointPtr;
			Point3d smoothedPt = Point3d(0, 0, 0);
			double weights = 0.0;
			std::shared_ptr<std::vector<Point3d*>> neighborhood = this->query(origin, radius);
			#pragma omp parallel for
			for (int i = 0; i < neighborhood->size(); ++i)
			{
				
				double wi = std::exp((-1.0 * distance3d(origin, *neighborhood->at(i))) / radius);
				smoothedPt += *neighborhood->at(i)*wi;
				weights += wi;
			}
			smoothedPt *= (1.0 / weights);
			smoothedPoints->push_back(smoothedPt);
		}
	);

	this->_storeCreatedPointData(smoothedPoints);
	std::shared_ptr<std::vector<Point3d*>> smoothedPointPtrs = util::computePointPtrVectorFrom(*smoothedPoints);
	std::shared_ptr<PointCloud3d> smoothedCloud = std::make_shared<PointCloud3d>(this->_storeCreatedPointData, *smoothedPointPtrs);
	smoothedCloud->setName("sc");
	return smoothedCloud;
}

std::shared_ptr<PointCloud3d> PointCloud3d::computeThinnedVersionWith(double thinningRadius) {

	std::vector<Point3d*> remainingPointPtrs;

	ThreeDTree & tree = this->_tree;
	tree.unhideAllNodes();

	tree.toEachLeafNodeApply(
		[&tree, &remainingPointPtrs, thinningRadius](LeafNode & leafNode)->void {
			
			if (leafNode.isHidden()) {
				// there is nothing to do
			}
			else {
				remainingPointPtrs.push_back(leafNode.pointPtr);

				ThreeDTree::QueryResult nearNodes = tree.query(*leafNode.pointPtr, thinningRadius);
				for each ( std::shared_ptr<InnerNode> nearInnerNodePtr in *nearNodes.innerNodePtrs) {
					ThreeDTree::hideSubtreeAt(nearInnerNodePtr, true);
				}
				for each (std::shared_ptr<LeafNode> nearLeafNodePtr in *nearNodes.leafNodePtrs) {
					nearLeafNodePtr->hide();
				}
			}
		}
	);

	tree.unhideAllNodes();
	std::shared_ptr<PointCloud3d> thinnedCloud = std::make_shared<PointCloud3d>(this->_storeCreatedPointData, remainingPointPtrs);
	thinnedCloud->setName("tc");
	return thinnedCloud;
}

