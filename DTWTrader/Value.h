#pragma once
#include <vector>
#include <iostream>

class Value : public std::vector<double> {
public:
	static const int DIM = 4;

public:
	Value() : std::vector<double>(DIM) { }

	Value(size_t size, double value) : std::vector<double>(size, value) { }

	void  Read(std::istream& istream) {
		std::vector<double>& x(*this);
		istream.read(reinterpret_cast<char*>(&(x[0])), DIM * sizeof(double));
	}

	void Write(std::ostream& ostream) const {
		const std::vector<double>& x(*this);
		ostream.write(reinterpret_cast<const char*>(&(x[0])), DIM * sizeof(double));
	}

	Value& operator+=(const Value& a0) {
		std::vector<double>& x(*this);
		for (int i = 0; i < DIM; ++i) {
			x[i] += a0[i];
		}
		return *this;
	}

	Value& operator/=(double d0) {
		std::vector<double>& x(*this);
		for (int i = 0; i < DIM; ++i) {
			x[i] /= d0;
		}
		return *this;
	}

	Value& operator=(double x0) {
		std::vector<double>& x(*this);
		for (auto i = 0; i < DIM; ++i) {
			x[i] = x0;
		}
		return *this;
	}
};
