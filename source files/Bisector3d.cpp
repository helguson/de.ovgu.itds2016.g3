#include "Bisector3d.h"

Bisector3d::Bisector3d(Dimension dimension, std::shared_ptr<LeafNode> medianElementPtr) 
	:dimension(dimension),
	medianElementPtr(medianElementPtr)
{
}

std::tuple<Interval3d, Interval3d> Bisector3d::bisect(const Interval3d& interval) const {

	Interval3d inferiorInterval(interval);
	Interval3d superiorInterval(interval);

	double medianValue;

	switch (this->dimension)
	{
	case Dimension::X:
		medianValue = this->medianElementPtr->pointPtr->x;
		inferiorInterval.x.superiorBorder = medianValue;
		inferiorInterval.x.superiorBorderIsContained = true;
		superiorInterval.x.inferiorBorder = medianValue;
		superiorInterval.x.inferiorBorderIsContained = true;
		break;
	case Dimension::Y:
		medianValue = this->medianElementPtr->pointPtr->y;
		inferiorInterval.y.superiorBorder = medianValue;
		inferiorInterval.y.superiorBorderIsContained = true;
		superiorInterval.y.inferiorBorder = medianValue;
		superiorInterval.y.inferiorBorderIsContained = true;
		break;
	case Dimension::Z:
		medianValue = this->medianElementPtr->pointPtr->z;
		inferiorInterval.z.superiorBorder = medianValue;
		inferiorInterval.z.superiorBorderIsContained = true;
		superiorInterval.z.inferiorBorder = medianValue;
		superiorInterval.z.inferiorBorderIsContained = true;
		break;
	default:
		// should not happen; TODO: make safe
		break;
	}

	return std::tuple<Interval3d, Interval3d>(inferiorInterval, superiorInterval);
}