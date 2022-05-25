#pragma once
#include <vector>
#include "GraphNode.h"

class Path {
public:
	int pathLength;
	int nuberOfNodes;
	double avergaeSequenceIdentity;
	std::vector<GraphNode*> pathNodes;

	bool CreateDeterministicPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead);
	bool CreateMonteCarloPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead);

private:
	bool RekurzCreateDeterministicPath(GraphNode* _currentNode);
	bool RekurzCreateMonteCarloPath(GraphNode* _currentNode);
};
