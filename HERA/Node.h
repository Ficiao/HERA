#pragma once
#include <vector>
#include "Connection.h"

class Node {
public:
	int index;
	int size;
	std::vector<Connection> connections;

};