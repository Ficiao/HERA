#include "Path.h"
#include "Connection.h"

using namespace std;

bool Path::CreateDeterministicPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead) {
	_contigNode->hasBeenUsed = true;

	//todo: dodati da se postavi na true i od komplementa

	//pathLength = pathLength + _contigNode->connections.at(_indexOfStartingRead).baseEnd;

	depth = 0;
	if (RekurzCreateDeterministicPath(_contigNode->connections.at(_indexOfStartingRead).target) == true) {
		pathNodes.insert(pathNodes.begin(), _contigNode);
		averageSequenceIdentity += _contigNode->connections.at(_indexOfStartingRead).sequenceIdentity;
		averageSequenceIdentity =(double) (averageSequenceIdentity / pathNodes.size());

		for (int i = 0; i < pathNodes.size(); i++) {
			pathNodes.at(i)->hasBeenUsed = false;
		}

		return true;
	}

	return false;;
}

bool Path::RekurzCreateDeterministicPath(GraphNode* _currentNode) {
	bool _pathCreated;
	depth++;

	if (depth > 5000) {
		return false;
	}
	_currentNode->hasBeenUsed = true;

	for (int i = 0; i < _currentNode->backwardsContigConnection.size(); i++) {
		if (_currentNode->backwardsContigConnection.at(i).target->hasBeenUsed == false) {
			pathNodes.insert(pathNodes.begin(), _currentNode->backwardsContigConnection.at(i).target);
			averageSequenceIdentity += _currentNode->backwardsContigConnection.at(i).sequenceIdentity;
			pathNodes.insert(pathNodes.begin(), _currentNode);
			averageSequenceIdentity += _currentNode->connections.at(i).sequenceIdentity;
			pathLength += _currentNode->size - _currentNode->connections.at(i).baseStart;
			//treba li dodati i duljinu contiga?
			return true;
		}
	}


	for (int i = 0; i < _currentNode->connections.size() && depth <= 5000; i++) {
		if (_currentNode->connections.at(i).target->hasBeenUsed == false) {
			_pathCreated = RekurzCreateDeterministicPath(_currentNode->connections.at(i).target);
			if (_pathCreated == true) {
				pathNodes.insert(pathNodes.begin(), _currentNode);
				averageSequenceIdentity += _currentNode->connections.at(i).sequenceIdentity;
				pathLength += _currentNode->size - _currentNode->connections.at(i).baseStart;
				return true;
			}
		}
	}

	_currentNode->hasBeenUsed = false;
	return false;
}

bool Path::CreateMonteCarloPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead) {

	return false;
}

bool Path::RekurzCreateMonteCarloPath(GraphNode* _currentNode) {

	return false;
}