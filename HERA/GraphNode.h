#pragma once
#include <vector>
#include <string>

class GraphNode {
public:
	int index;
	int size;
	bool isContig;
	bool hasBeenUsed;
    std::string read;
	std::vector<class Connection> connections;
	std::vector<class Connection> backwardsContigConnection;
};