#pragma once
#include "Bucket.h"

class Buckets {
public: 
	std::vector<Bucket> buckets;

	Buckets(int _numberOfContigs) {
		for (int i = 1; i < _numberOfContigs; i++) {
			for (int j = 1; j <= _numberOfContigs; j++) {
				if (i <= _numberOfContigs / 2) {
					if (j != i) {
						buckets.push_back(*new Bucket(i, j));
					}
				}
				else {
					if (j != i) {
						buckets.push_back(*new Bucket(i, j));
					}
				}
			}			
		}
	}

	void FillBucketsDeterministic(GraphNode* _readNodes, GraphNode* _contigNodes, int _numberOfContigNodes);
	void FillBucketsMonteCarlo(GraphNode* _readNodes, GraphNode* _contigNodes, int _numberOfContigNodes);
	std::vector<Path> SelectWinner(int numberOfContigs);
};
