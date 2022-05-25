#include "Path.h"
#include "Connection.h"

using namespace std;

bool Path::CreateDeterministicPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead) {
	pathNodes.push_back(_contigNode);
	pathLength = pathLength + _contigNode->connections.at(_indexOfStartingRead).baseEnd;


	return false;
}

bool Path::RekurzCreateDeterministicPath(GraphNode* _currentNode) {

	return false;
}

bool Path::CreateMonteCarloPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead) {

	return false;
}

bool Path::RekurzCreateMonteCarloPath(GraphNode* _currentNode) {

	return false;
}