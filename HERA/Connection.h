#pragma once

class Connection {
public:
	class GraphNode* base;
	int baseStart;
	int baseEnd;
	class GraphNode* target;
	int targetStart;
	int targetEnd;
	int residueMatches;
	int allignmentBlockLength;
	double overlapScore;
	double extensionScore;
	double sequenceIdentity;
};