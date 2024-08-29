#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Value.h"

class Vector : public std::vector<Value> {
public:
	static void Initialize(int colums, int excludes) {
		COLUMNS = colums;
		EXCLUDES = excludes;
		DIMENSION = colums - excludes;
	}

	Vector() : std::vector<Value>(COLUMNS), label(-1) { }

	void  Read(std::istream& istream) {
		std::vector<double>& x(*this);
		istream.read(reinterpret_cast<char*>(&(x[0])), Vector::COLUMNS * sizeof(double));
		label = -1;
	}

	void Write(std::ostream& ostream) const {
		const std::vector<double>& x(*this);
		ostream.write(reinterpret_cast<const char*>(&(x[0])), Vector::COLUMNS * sizeof(double));
	}

	friend double Dist(const Vector& a, const Vector& b) {
		double sum = 0;
		for (int i = 0; i < DIMENSION; ++i) {
			sum += (a[i] - b[i]) * (a[i] - b[i]);
		}
		return std::sqrt(sum);
	}

	Vector& operator+=(const Vector& a0) {
		std::vector<Value>& x(*this);
		#pragma omp parallel for
		for (int i = 0; i < COLUMNS; ++i) {
			x[i] += a0[i];
		}
		return *this;
	}

	Vector& operator/=(double d0) {
		std::vector<Value>& x(*this);
		#pragma omp parallel for
		for (int i = 0; i < COLUMNS; ++i) {
			x[i] /= d0;
		}
		return *this;
	}

	Vector& operator=(double x0) {
		std::vector<Value>& x(*this);
		#pragma omp parallel for
		for (auto i = 0; i < COLUMNS; ++i) {
			x[i] = x0;
		}
		return *this;
	}

public:
	int label;

public:
	static int COLUMNS;
	static int EXCLUDES;
	static int DIMENSION;
};

int Vector::COLUMNS;
int Vector::EXCLUDES;
int Vector::DIMENSION;
