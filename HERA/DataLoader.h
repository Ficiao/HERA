#pragma once
#include <string>
#include "GraphNode.h"

class DataLoader {
public:
	GraphNode* LoadData(std::string _fileName, GraphNode* _targetNodes, bool _isContig);
private:
	int IndexOfLastElem(std::string _fileName);
	void FillNodes(GraphNode* _nodes, GraphNode* _targetNodes, std::string _fileName, bool _isContig);
	int GetIndexFromName(std::string _name);
};