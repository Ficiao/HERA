#pragma once
#include "Node.h"

class Connection {
public:
	Node* _base;
	int _baseStart;
	int _baseEnd;
	char _relativeStrand;
	Node* _target;
	int _targetStart;
	int _targetEnd;
	int _residueMatches;
	int _allignmentBlockLenth;
};