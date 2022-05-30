// @Author Matanović

#pragma once
#include <string>
#include "GraphNode.h"

class DataLoader {
public:
	GraphNode* LoadData(std::string _fileName, GraphNode* _targetNodes, bool _isContig, int _numberOfNodes);
    int GetIndexFromName(std::string _name);
private:
    void FillNodes(GraphNode* _nodes, GraphNode* _targetNodes, std::string _fileName, bool _isContig);
};