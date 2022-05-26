#include "Path.h"
#include "Connection.h"

using namespace std;

bool Path::CreateDeterministicPath(GraphNode* _readNodes, GraphNode* _contigNode, int _indexOfStartingRead) {
	_contigNode->hasBeenUsed = true;

	//todo: dodati da se postavi na true i od komplementa

	//pathLength = pathLength + _contigNode->connections.at(_indexOfStartingRead).baseEnd;

	pathNodes.insert(pathNodes.begin(), _contigNode);

	if (RekurzCreateDeterministicPath(_contigNode->connections.at(_indexOfStartingRead).target) == true) {
		pathNodes.insert(pathNodes.begin(), _contigNode);
		averageSequenceIdentity = _contigNode->connections.at(_indexOfStartingRead).sequenceIdentity;
		nuberOfNodes++;
		averageSequenceIdentity =(double) (averageSequenceIdentity / nuberOfNodes);

		for (int i = 0; i < pathNodes.size(); i++) {
			pathNodes.at(i)->hasBeenUsed = false;
		}

		return true;
	}

	return false;;
}

bool Path::RekurzCreateDeterministicPath(GraphNode* _currentNode) {
	bool _pathCreated;

	_currentNode->hasBeenUsed = true;

	for (int i = 0; i < _currentNode->backwardsContigConnection.size(); i++) {
		if (_currentNode->backwardsContigConnection.at(i).target->hasBeenUsed == false) {
			pathNodes.insert(pathNodes.begin(), _currentNode->backwardsContigConnection.at(i).target);
			averageSequenceIdentity += _currentNode->backwardsContigConnection.at(i).sequenceIdentity;
			nuberOfNodes++;
			//treba li dodati i duljinu contiga?
			return true;
		}
	}


	for (int i = 0; i < _currentNode->connections.size(); i++) {
		if (_currentNode->connections.at(i).target->hasBeenUsed == false) {
			_pathCreated = RekurzCreateDeterministicPath(_currentNode->connections.at(i).target);
			if (_pathCreated == true) {
				pathNodes.insert(pathNodes.begin(), _currentNode);
				averageSequenceIdentity += _currentNode->connections.at(i).sequenceIdentity;
				nuberOfNodes++;
				pathLength += _currentNode->size - _currentNode->connections.at(i).baseEnd;
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