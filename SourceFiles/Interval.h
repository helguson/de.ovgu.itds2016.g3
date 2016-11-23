#pragma once

#include <string>

struct Interval {
public:
	double inferiorBorder;
	double superiorBorder;
	bool inferiorBorderIsContained;
	bool superiorBorderIsContained;

	Interval(double inferiorBorder = -std::numeric_limits<double>::infinity(), bool inferiorBorderIsContained = false, double superiorBorder = std::numeric_limits<double>::infinity(), bool superiorBorderIsContained = false);

	bool contains(double value) const;
	bool contains(const Interval& interval) const;
	bool intersectsWith(const Interval& interval) const;
	bool isInferiorTo(double value) const;
	bool isInferiorTo(const Interval& interval) const;
	bool isSuperiorTo(double value) const;
	bool isSuperiorTo(const Interval& interval) const;
	std::string toString() const;
	bool isValid() const;
	bool isEmpty() const;
	double range() const;

	static void runTest();
	static Interval all(); // returns (-infinity, +infinity)
private:
	bool containsAccordingToInferiorBorder(double value) const;
	bool containsAccordingToInferiorBorder(double borderValue, bool borderIsContained) const;
	bool containsAccordingToSuperiorBorder(double value) const;
	bool containsAccordingToSuperiorBorder(double borderValue, bool borderIsContained) const;
};