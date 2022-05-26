#pragma once
#include "Path.h"

class Bucket {
public:
	int startContigIndex;
	int endContigIndex;
	std::vector<Path> paths;
    int winningPathIndex;

	Bucket(int _startContigIndex, int _endContigIndex) {
		startContigIndex = _startContigIndex;
		endContigIndex = _endContigIndex;
	}
};
