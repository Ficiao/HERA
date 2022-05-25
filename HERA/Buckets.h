#pragma once
#include <vector>
#include "Path.h"

class Buckets {
public: 
	int _averageBucketLength1SequenceIdentity;
	int _averageBucketLength2SequenceIdentity;
	std::vector<Path> _bucketLength1;
	std::vector<Path> _bucketLength2;

	void FillBucketsDeterministic(GraphNode* _readNodes, GraphNode* _contigNode, int _numberOfContigNodes);
	void FillBucketsMonteCarlo(GraphNode* _readNodes, GraphNode* _contigNode, int _numberOfContigNodes, int _monteCarloHyperparameter);
	Path* SelectWinner();
};
