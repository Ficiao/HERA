#pragma once
#include <vector>
#include "GraphNode.h"

class Path {
public:
	int pathLength;
	int nuberOfNodes;
	double averageSequenceIdentity;
	std::vector<GraphNode*> pathNodes;

	Path() {
		pathLength = 0;
		nuberOfNodes = 0;
		averageSequenceIdentity = -1;
	}

	bool CreateDeterministicPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead);
	bool CreateMonteCarloPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead);

private:
	bool RekurzCreateDeterministicPath(GraphNode* _currentNode);
	bool RekurzCreateMonteCarloPath(GraphNode* _currentNode);
};
