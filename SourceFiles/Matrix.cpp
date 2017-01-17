#include "Matrix.h"
#include "SVD.h"


Matrix Matrix::operator* (Matrix const & right) const {
	Matrix const & L = *this;
	Matrix const & R = right;

	size_t m = L.M();
	size_t n = L.N();
	size_t n2 = R.M();
	size_t o = R.N();

	if (n != n2) {
		throw std::invalid_argument("matrix dimensions do not match");
	}
	else {
		Matrix result(m, o);

		for (size_t iM = 0; iM < m; iM++) {
			for (size_t iO = 0; iO < o; iO++) {
				for (size_t iN = 0; iN < n; iN++) {
					result(iM, iO) += L(iM, iN)*R(iN, iO);
				}
			}
		}

		return result;
	}
}

Matrix Matrix::transpose() const {
	size_t m = this->M();
	size_t n = this->N();
	Matrix result(n, m);

	for (size_t iM = 0; iM < m; iM++) {
		for (size_t iN = 0; iN < n; iN++) {
			result(iN, iM) = (*this)(iM, iN);
		}
	}

	return result;
}

Matrix Matrix::solveFor(Matrix const & B) const {
	Matrix const & A = *this;

	size_t m = A.M();
	size_t n = A.N();
	size_t m2 = B.M();
	size_t o = B.N();

	if (m != m2) {
		throw std::invalid_argument("matrix dimensions do not match");
	}
	else {
		Matrix pseudoInverseOfA = A.calculatePseudoInverse();
		return pseudoInverseOfA*B;
	}
}

Matrix Matrix::calculatePseudoInverse() const {
	
	Matrix U = *this;
	size_t n = U.N();
	std::vector<double> singularValues;
	Matrix V(n, n);

	SVD::decomposeMatrix(U, singularValues, V);	// A = U*S*V^T

	Matrix S_1(n, n);
	double const EPSILON = pow(10, -9);
	for (size_t i = 0; i < n; i++) {
		double singularValue = singularValues[i];
		S_1(i, i) = (singularValue > EPSILON) ? 1 / singularValue : 0;
	}

	Matrix Ut = U.transpose();
	
	return V*S_1*Ut;
}

void Matrix::runTest() {

	Matrix A(2, 2);
	A(0, 0) = 1; A(0, 1) = 1;	// 1 1
	A(1, 0) = 0; A(1, 1) = 1;	// 0 1
	
	

	Matrix B(2, 2);
	B(0, 0) = 1; B(0, 1) = 2;	// 1 2
	B(1, 0) = 3; B(1, 1) = 4;	// 3 4

	Matrix C = A*B;	// 4 6
					// 3 4

	Matrix D = B.transpose();	// 1 3
								// 2 4

	Matrix E = B.calculatePseudoInverse()*B;

	Matrix x(2, 1);
	x[0] = 1;
	x[1] = 2;

	Matrix b = A*x;

	// Matrix c = x*A;	// throws exception
}