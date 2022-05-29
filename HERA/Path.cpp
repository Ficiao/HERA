#include <stdio.h>
#include "Path.h"
#include "Connection.h"
#include <time.h>

using namespace std;

bool Path::CreateDeterministicPath(GraphNode *_readNodes, GraphNode *_contigNode, int _indexOfStartingRead) {
    _contigNode->hasBeenUsed = true;

    depth = 0;
    if (RekurzCreateDeterministicPath(_contigNode->connections.at(_indexOfStartingRead).target, _contigNode->index) == true) {
        pathNodes.insert(pathNodes.begin(), _contigNode);
        averageSequenceIdentity += _contigNode->connections.at(_indexOfStartingRead).sequenceIdentity;
        averageSequenceIdentity = (double) (averageSequenceIdentity / (double)(pathNodes.size() - 1));

        for (int i = 0; i < pathNodes.size(); i++) {
            pathNodes.at(i)->hasBeenUsed = false;
        }

        return true;
    }

    _contigNode->hasBeenUsed = false;
    return false;
}

bool Path::RekurzCreateDeterministicPath(GraphNode *_currentNode, int _currentContig) {
    bool _pathCreated;
    depth++;

    if (depth > 50000) {
        return false;
    }
    _currentNode->hasBeenUsed = true;

    for (int i = 0; i < _currentNode->backwardsContigConnection.size(); i++) {
        if (_currentNode->backwardsContigConnection.at(i).target->index == _currentContig + 1) {
            pathNodes.insert(pathNodes.begin(), _currentNode->backwardsContigConnection.at(i).target);
            averageSequenceIdentity += _currentNode->backwardsContigConnection.at(i).sequenceIdentity;
            pathNodes.insert(pathNodes.begin(), _currentNode);
            pathLength += _currentNode->backwardsContigConnection.at(i).baseStart;
            return true;
        }
    }


    for (int i = 0; i < _currentNode->connections.size() && depth <= 50000; i++) {
        if (_currentNode->connections.at(i).target->hasBeenUsed == false) {
            _pathCreated = RekurzCreateDeterministicPath(_currentNode->connections.at(i).target, _currentContig);
            if (_pathCreated == true) {
                pathNodes.insert(pathNodes.begin(), _currentNode);
                averageSequenceIdentity += _currentNode->connections.at(i).sequenceIdentity;
                pathLength += _currentNode->connections.at(i).baseStart;
                return true;
            }
        }
    }

    _currentNode->hasBeenUsed = false;
    return false;
}

bool Path::CreateMonteCarloPath(GraphNode *_readNodes, GraphNode *_contigNode) {
    _contigNode->hasBeenUsed = true;
    averageSequenceIdentity = 0;

    depth = 0;
    if (RekurzCreateMonteCarloPath(_contigNode)) {
        averageSequenceIdentity = (double) (averageSequenceIdentity / (pathNodes.size() - 1));

        for (int i = 0; i < pathNodes.size(); i++) {
            pathNodes.at(i)->hasBeenUsed = false;
        }

        return true;
    }

    _contigNode->hasBeenUsed = false;
    return false;
}

bool Path::RekurzCreateMonteCarloPath(GraphNode *_currentNode) {
    bool _pathCreated;
    depth++;

    if (depth > 100000) {
        return false;
    }

    _currentNode->hasBeenUsed = true;

    for (int i = 0; i < _currentNode->backwardsContigConnection.size(); i++) {
        if (!_currentNode->backwardsContigConnection.at(i).target->hasBeenUsed) {
            pathNodes.insert(pathNodes.begin(), _currentNode->backwardsContigConnection.at(i).target);
            averageSequenceIdentity += _currentNode->backwardsContigConnection.at(i).sequenceIdentity;
            pathNodes.insert(pathNodes.begin(), _currentNode);
            //averageSequenceIdentity += _currentNode->connections.at(i).sequenceIdentity;
            //pathLength += _currentNode->connections.at(i).baseStart;
            return true;
        }
    }

    int _extensionScoreSum = 0;
    for (int i = 0; i < _currentNode->connections.size(); i++) {
        _extensionScoreSum += _currentNode->connections.at(i).extensionScore;
    }

    srand(time(0));

    for (int i = 0; i < _currentNode->connections.size() * 5 && depth <= 100000; i++) {

        int _random = (rand() % (_extensionScoreSum + 1));
        int _index;
        int _currentSum=0;

        for (_index = 0; _index < _currentNode->connections.size() - 1; _index++) {
            if (_random <= _currentSum + _currentNode->connections.at(_index).extensionScore) {
                break;
            }
            _currentSum += _currentNode->connections.at(_index).extensionScore;
        }

        if (_currentNode->connections.at(_index).target->hasBeenUsed == false) {
            _pathCreated = RekurzCreateMonteCarloPath(_currentNode->connections.at(_index).target);
            if (_pathCreated == true) {
                pathNodes.insert(pathNodes.begin(), _currentNode);
                averageSequenceIdentity += _currentNode->connections.at(i).sequenceIdentity;
                if (_currentNode->isContig == false) {
                    pathLength += _currentNode->connections.at(i).baseStart;
                }
                return true;
            }
        }
    }

    _currentNode->hasBeenUsed = false;
    return false;
}