#pragma once
#include <vector>
#include "Path.h"

class Bucket {
public:
	int startContigIndex;
	int endContigIndex;
	std::vector<Path> paths;

	Bucket(int _startContigIndex, int _endContigIndex) {
		startContigIndex = _startContigIndex;
		endContigIndex = _endContigIndex;
	}
};
