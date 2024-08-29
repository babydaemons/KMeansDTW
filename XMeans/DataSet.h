#pragma once
#include <fstream>
#include "Vector.h"

class DataSet : public std::vector<Vector> {
public:
	DataSet() {	}

	void Read(std::istream& istream) {
		while (!istream.eof()) {
			Vector a;
			a.Read(istream);
			push_back(a);
		}
	}

	void Write(std::ostream& ostream) {
		for (size_t i = 0; i < size(); ++i) {
			at(i).Write(ostream);
		}
	}
};