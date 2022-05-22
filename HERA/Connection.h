#pragma once

class Connection {
public:
	class GraphNode* _base;
	int _baseStart;
	int _baseEnd;
	char _relativeStrand;
	class GraphNode* _target;
	int _targetStart;
	int _targetEnd;
	int _residueMatches;
	int _allignmentBlockLenth;
};