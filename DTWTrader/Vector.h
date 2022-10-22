#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "DTW.h"

class Vector : public std::vector<Value> {
public:
	static void Initialize(int colums, int excludes) {
		COLUMNS = colums;
		EXCLUDES = excludes;
		DIMENSION = colums - excludes;
	}

	Vector() : std::vector<Value>(COLUMNS), label(-1) { }

	void  Read(std::istream& istream) {
		std::vector<Value>& x(*this);
		for (size_t i = 0; x.size(); ++i) {
			x[i].Read(istream);
		}
		label = -1;
	}

	void Write(std::ostream& ostream) const {
		const std::vector<Value>& x(*this);
		for (size_t i = 0; x.size(); ++i) {
			x[i].Write(ostream);
		}
	}

	friend double Dist(const Vector& a, const Vector& b) {
		return DTW::dtw_distance_only(a, b, 2.0);
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

private:
	static int COLUMNS;
	static int EXCLUDES;
	static int DIMENSION;
};

int Vector::COLUMNS;
int Vector::EXCLUDES;
int Vector::DIMENSION;
