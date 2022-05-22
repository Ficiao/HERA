#pragma once
#include <string>
#include "ReadNode.h"

class DataLoader {
public:
	Node* LoadData(std::string _fileName, Node* _targetNodes);
private:
	int IndexOfLastElem(std::string _fileName);
	void FillNodes(Node* _nodes, Node* _targetNodes, std::string _fileName);
	int GetIndexFromName(std::string _name);
};