#include <stdio.h>
#include "Path.h"
#include "Connection.h"
#include "Range.h"
#include <time.h>
#include <random>
#include <algorithm>

using namespace std;

bool Path::CreateDeterministicPath(GraphNode *_readNodes, GraphNode *_contigNode, int _indexOfStartingRead) {
    _contigNode->hasBeenUsed = true;

    depth = 0;
    if (RekurzCreateDeterministicPath(_contigNode->connections.at(_indexOfStartingRead).target, _contigNode->index) ==
        true) {
        pathNodes.insert(pathNodes.begin(), _contigNode);
        averageSequenceIdentity += _contigNode->connections.at(_indexOfStartingRead).sequenceIdentity;
        averageSequenceIdentity = (double) (averageSequenceIdentity / (double) (pathNodes.size() - 1));

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
    if (RekurzCreateMonteCarloPath(_contigNode, _contigNode->index)) {
        pathNodes.insert(pathNodes.begin(), _contigNode);
        averageSequenceIdentity = (double) (averageSequenceIdentity / (pathNodes.size() - 1));

        for (auto &pathNode: pathNodes) {
            pathNode->hasBeenUsed = false;
        }

        return true;
    }

    _contigNode->hasBeenUsed = false;
    return false;
}

bool Path::RekurzCreateMonteCarloPath(GraphNode *_currentNode, int _currentContig) {
    bool _pathCreated;
    depth++;

    if (depth > 100000) {
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

    bool _allUsed = std::all_of(_currentNode->connections.begin(),
                                _currentNode->connections.end(),
                                [](Connection &connection) { return connection.target->hasBeenUsed; });

    if (_currentNode->connections.empty() || _allUsed) {
        return false;
    }

    int _extensionScoreSum = std::accumulate(_currentNode->connections.begin(), _currentNode->connections.end(), 0,
                                             [](int i, const Connection &connection) {
                                                 return int(connection.extensionScore) + i;
                                             });

    std::vector<Range> probabilites;
    int start = 0;

    // assign ranges to connections depending on their extension score
    for (auto &connection: _currentNode->connections) {
        int end = start + int(connection.extensionScore);
        Range range = Range(start, end, connection);
        probabilites.push_back(range);
        start = end;
    }

    while (depth <= 100000) {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<int> distr(0, _extensionScoreSum); // define the range

        // generate random number
        int _random = distr(gen);

        // find the right range
        auto it = std::find_if(probabilites.begin(), probabilites.end(), [_random](Range range) {
            return range.contains(_random);
        });
        long index = std::distance(probabilites.begin(), it);


//        printf("Found Range with index %ld and %d - %d, random num %d, ex sum %d\n", index, probabilites[index].start,
//               probabilites[index].end, _random, _extensionScoreSum);
        if (index > probabilites.size() - 1) {
            continue;
        }

        if (index == 0) {
            int test = 1;
        }

        Connection connection = probabilites[index].connection;

        if (!connection.target->hasBeenUsed) {
            _pathCreated = RekurzCreateMonteCarloPath(connection.target, _currentContig);
            if (_pathCreated) {
                pathNodes.insert(pathNodes.begin(), _currentNode);
                averageSequenceIdentity += connection.sequenceIdentity;
                if (!_currentNode->isContig) {
                    pathLength += connection.baseStart;
                }
                return true;
            }
        } else {
            depth++;

            bool _allUsed = std::all_of(_currentNode->connections.begin(),
                                        _currentNode->connections.end(),
                                        [](Connection &connection) { return connection.target->hasBeenUsed; });

            if (_allUsed) {
                return false;
            }
        }
    }

    _currentNode->hasBeenUsed = false;
    return false;
}