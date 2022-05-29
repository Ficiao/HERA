#pragma once
#include "GraphNode.h"

class Path {
public:
	int pathLength;
	int depth;
	double averageSequenceIdentity;
	std::vector<GraphNode*> pathNodes;

	Path() {
		depth = 0;
		pathLength = 0;
		averageSequenceIdentity = -1;
	}

	bool CreateDeterministicPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead);
	bool CreateMonteCarloPath(GraphNode* _readNodes, GraphNode* _contigNode);

private:
	bool RekurzCreateDeterministicPath(GraphNode* _currentNode, int _currentContig);
	bool RekurzCreateMonteCarloPath(GraphNode* _currentNode);
};
