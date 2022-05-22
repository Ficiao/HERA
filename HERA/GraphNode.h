#pragma once
#include <vector>

class GraphNode {
public:
	int index;
	int size;
	std::vector<class Connection> connections;
};