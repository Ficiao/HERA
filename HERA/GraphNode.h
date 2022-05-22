#pragma once
#include <vector>

class GraphNode {
public:
	int index;
	int size;
	bool isContig;
	std::vector<class Connection> connections;
	std::vector<class Connection> backwardsContigConnection;
};