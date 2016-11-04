#include "Interval.h"

#include <iostream>
#include <sstream>

void log(std::string message) {
	std::cout << message << std::endl;
}

std::string toStr(bool value) {
	std::string result;

	if (value == true) {
		result = "true";
	}
	else {
		result = "false";
	}
	return result;
}

void test01_toString() {
	log("####################");
	log("### start test01 ###");
	log("####################");
	Interval i1(0, true, 3, true); // [0, 3]
	Interval i2(-1, false, 0, false); // (-1, 0)

	log("interval should be [0,3] and is " + i1.toString());
	log("interval should be (-1,0) and is " + i2.toString());
}

void test02_contains() {
	log("####################");
	log("### start test02 ###");
	log("####################");
	Interval i1(0, true, 3, true); // [0, 3]
	Interval i2(-1, false, 1, false); // (-1, 1)

	log(i1.toString() + " contains 0: " + toStr(i1.contains(0)));
	log(i1.toString() + " contains 1: " + toStr(i1.contains(1)));
	log(i1.toString() + " contains 3: " + toStr(i1.contains(3)));
	log(i1.toString() + " contains 4: " + toStr(i1.contains(4)));

	log(i2.toString() + " contains -1: " + toStr(i2.contains(-1)));
	log(i2.toString() + " contains 0: " + toStr(i2.contains(0)));
	log(i2.toString() + " contains 1: " + toStr(i2.contains(1)));

	Interval i3(0, true, 1, true); // [0, 1]
	Interval i4(0, false, 1, false); // (0, 1)
	Interval i5(0, true, 3, true); // [0, 3]
	Interval i6(1, true, 2, true); // [1, 2]
	Interval i7(0, true, 2, true); // [0, 2]
	Interval i8(1, true, 3, true); // [1, 3]


	log(i3.toString() + " contains "+ i4.toString() +": " + toStr(i3.contains(i4)));
	log(i4.toString() + " contains " + i3.toString() + ": " + toStr(i4.contains(i3)));
	log(i4.toString() + " contains " + i4.toString() + ": " + toStr(i4.contains(i4)));
	log(i5.toString() + " contains " + i6.toString() + ": " + toStr(i5.contains(i6)));
	log(i6.toString() + " contains " + i5.toString() + ": " + toStr(i6.contains(i5)));
	log(i7.toString() + " contains " + i8.toString() + ": " + toStr(i7.contains(i8)));
	log(i8.toString() + " contains " + i7.toString() + ": " + toStr(i8.contains(i7)));
}

void test03_intersectsWith() {
	log("####################");
	log("### start test03 ###");
	log("####################");
	Interval i1(0, true, 1, true); // [0, 1]
	Interval i2(1, false, 3, false); // (1, 3)
	Interval i3(2, true, 3, true); // [2, 3]

	log(i1.toString() + " intersects with " + i2.toString() + ":" + toStr(i1.intersectsWith(i2)));
	log(i2.toString() + " intersects with " + i3.toString() + ":" + toStr(i2.intersectsWith(i3)));
	log(i1.toString() + " intersects with " + i3.toString() + ":" + toStr(i1.intersectsWith(i3)));
}

void test04_isInferiorTo() {
	log("####################");
	log("### start test04 ###");
	log("####################");
	Interval i1(0, true, 1, true); // [0, 1]
	Interval i2(0, false, 1, false); // (0, 1)

	log(i1.toString() + " is inferior to 1: " + toStr(i1.isInferiorTo(1)));
	log(i2.toString() + " is inferior to 1: " + toStr(i2.isInferiorTo(1)));


	Interval i3(0, true, 1, true); // [0, 1]
	Interval i4(1, true, 2, true); // [1, 2]
	Interval i5(1, false, 2, false); // (1, 2)
	Interval i6(2, true, 3, true); // [2, 3]

	log(i3.toString() + " is inferior to " + i4.toString() + ":" + toStr(i3.isInferiorTo(i4)));
	log(i3.toString() + " is inferior to " + i5.toString() + ":" + toStr(i3.isInferiorTo(i5)));
	log(i3.toString() + " is inferior to " + i6.toString() + ":" + toStr(i3.isInferiorTo(i6)));
}


void test05_isSuperiorTo() {
	log("####################");
	log("### start test05 ###");
	log("####################");
	Interval i1(0, true, 1, true); // [0, 1]
	Interval i2(0, false, 1, false); // (0, 1)

	log(i1.toString() + " is superior to 0: " + toStr(i1.isSuperiorTo(0)));
	log(i2.toString() + " is superior to 0: " + toStr(i2.isSuperiorTo(0)));


	Interval i3(0, true, 1, true); // [0, 1]
	Interval i4(-1, true, 0, true); // [-1, 0]
	Interval i5(-1, false, 0, false); // (-1, 0)
	Interval i6(-2, true, -1, true); // [-2, -1]

	log(i3.toString() + " is superior to " + i4.toString() + ":" + toStr(i3.isSuperiorTo(i4)));
	log(i3.toString() + " is superior to " + i5.toString() + ":" + toStr(i3.isSuperiorTo(i5)));
	log(i3.toString() + " is superior to " + i6.toString() + ":" + toStr(i3.isSuperiorTo(i6)));
}

void Interval::runTest() {

	test01_toString();
	test02_contains();
	test03_intersectsWith();
	test04_isInferiorTo();
	test05_isSuperiorTo();
}

Interval::Interval(double inferiorBorder, bool inferiorBorderIsContained, double superiorBorder, bool superiorBorderIsContained)
	:inferiorBorder(inferiorBorder),
	inferiorBorderIsContained(inferiorBorderIsContained),
	superiorBorder(superiorBorder),
	superiorBorderIsContained(superiorBorderIsContained)
{
}


bool Interval::containsAccordingToInferiorBorder(double value) const {
	if (this->inferiorBorderIsContained) {
		return this->inferiorBorder <= value;
	}
	else {
		return this->inferiorBorder < value;
	}
}

bool Interval::containsAccordingToSuperiorBorder(double value) const {
	if (this->superiorBorderIsContained) {
		return value <= this->superiorBorder;
	}
	else {
		return value < this->superiorBorder;
	}
}

bool Interval::contains(double value) const{
	return this->containsAccordingToInferiorBorder(value) 
		&& this->containsAccordingToSuperiorBorder(value);
}

bool Interval::containsAccordingToInferiorBorder(double borderValue, bool borderIsContained) const {

	if (this->inferiorBorder != borderValue) {
		return this->containsAccordingToInferiorBorder(borderValue);
	}
	else {
		return !(!this->inferiorBorderIsContained && borderIsContained);
	}
}

bool Interval::containsAccordingToSuperiorBorder(double borderValue, bool borderIsContained) const {

	if (this->superiorBorder != borderValue) {
		return this->containsAccordingToSuperiorBorder(borderValue);
	}
	else {
		return !(!this->superiorBorderIsContained && borderIsContained);
	}
}

bool Interval::contains(const Interval& interval) const {
	return this->containsAccordingToInferiorBorder(interval.inferiorBorder, interval.inferiorBorderIsContained)
		&& this->containsAccordingToSuperiorBorder(interval.superiorBorder, interval.superiorBorderIsContained);
}

bool Interval::intersectsWith(const Interval& interval) const {

	return !this->isInferiorTo(interval) && !this->isSuperiorTo(interval);
}

bool Interval::isInferiorTo(double value) const {

	return !this->containsAccordingToSuperiorBorder(value);
}

bool Interval::isInferiorTo(const Interval& interval) const {

	if (this->superiorBorder != interval.inferiorBorder) {
		return this->isInferiorTo(interval.inferiorBorder);
	}
	else {
		return !(this->superiorBorderIsContained && interval.inferiorBorderIsContained);
	}
}

bool Interval::isSuperiorTo(double value) const {

	return !this->containsAccordingToInferiorBorder(value);
}

bool Interval::isSuperiorTo(const Interval& interval) const {

	if (interval.superiorBorder != this->inferiorBorder) {
		return this->isSuperiorTo(interval.superiorBorder);
	}
	else {
		return !(interval.superiorBorderIsContained && this->inferiorBorderIsContained);
	}
}

std::string Interval::toString() const {

	std::string result;

	result += (this->inferiorBorderIsContained) ? "[" : "(";
	result += std::to_string(this->inferiorBorder);
	result += ", ";
	result += std::to_string(this->superiorBorder);
	result += (this->superiorBorderIsContained) ? "]" : ")";

	return result;
}

bool Interval::isValid() const {
	return this->inferiorBorder <= this->superiorBorder;
}

bool Interval::isEmpty() const {
	return (this->inferiorBorder == this->superiorBorder)
		&& !(this->inferiorBorderIsContained && this->superiorBorderIsContained);
}

double Interval::range() const {
	return this->superiorBorder - this->inferiorBorder;
}