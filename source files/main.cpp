#include "Controller.h"
#include "Interval.h"

#include "LeafNode.h"
#include "InnerNode.h"
#include "Point3d.h"
#include <memory>


void test() {

	Point3d p1(1, 0, 0);
	Point3d p2(2, 0, 0);

	Interval3d i = Interval3d::all();

	std::shared_ptr<LeafNode> l1p = std::make_shared<LeafNode>();
	std::shared_ptr<LeafNode> l2p = std::make_shared<LeafNode>();

	l1p->pointPtr = &p1;
	l2p->pointPtr = &p2;

	std::shared_ptr<InnerNode> rootp = std::make_shared<InnerNode>(i);

	rootp->bisector.dimension = Dimension::X;
	rootp->bisector.medianElementPtr = l1p;

	rootp->inferiorChild = l1p;
	rootp->superiorChild = l2p;

	std::tuple<Interval3d, Interval3d> childIntervals = rootp->bisector.bisect(i);

	rootp->inferiorChild->volume = std::get<0>(childIntervals);
	rootp->superiorChild->volume = std::get<1>(childIntervals);

}

int main() {
	
	//Interval::runTest();
	test();
	
	Controller controller;
	controller.startMainLoop();

	return 0;
}