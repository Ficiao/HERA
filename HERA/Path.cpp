#include <stdio.h>
#include "Path.h"
#include "Connection.h"
#include "Range.h"
#include <time.h>
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;

//Method used for path creation using greedy algorithm, expects Node connections to be sorted based on wanted value
bool Path::CreateDeterministicPath(GraphNode *_contigNode, int _indexOfStartingRead) {
    _contigNode->hasBeenUsed = true;

    depth = 0;

    //If creation of path was successfully, add starting contig to it and reset all elements as if they haven't been used
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

//Recursive method for path creation using greedy algorithm. For current node visit his connected nodes sequentially. 
//First time a node has connection to contig that hasn't been used, add it as path end and start recursively adding elements back to the path
bool Path::RekurzCreateDeterministicPath(GraphNode *_currentNode, int _currentContig) {
    bool _pathCreated;
    depth++;

    //If too many nodes have been visited consider creation unsuccessful
    if (depth > 50000) {
        return false;
    }
    _currentNode->hasBeenUsed = true;

    //Check if current node has connections to contigs that haven't been used, and if there is finish path creation
    for (int i = 0; i < _currentNode->backwardsContigConnection.size(); i++) {
        if (_currentNode->backwardsContigConnection.at(i).target->index == _currentContig + 1) {
            pathNodes.insert(pathNodes.begin(), _currentNode->backwardsContigConnection.at(i).target);
            averageSequenceIdentity += _currentNode->backwardsContigConnection.at(i).sequenceIdentity;
            pathNodes.insert(pathNodes.begin(), _currentNode);
            pathLength += _currentNode->backwardsContigConnection.at(i).baseStart;
            return true;
        }
    }

    //Sequentially visit all connected read nodes
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

//Method used for path creation using Monte Carlo algorithm, expects Node connections to be sorted based on wanted value
bool Path::CreateMonteCarloPath(GraphNode *_contigNode) {
    _contigNode->hasBeenUsed = true;
    averageSequenceIdentity = 0;
    depth = 0;

    //If creation of path was successfully, add starting contig to it and reset all elements as if they haven't been used
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

//Recursive method for path creation using Monte Carlo algorithm. For current node visit his connected node picked by random number generator with a probability of his extension score.
//First time a node has connection to contig that hasn't been used, add it as path end and start recursively adding elements back to the path
bool Path::RekurzCreateMonteCarloPath(GraphNode *_currentNode, int _currentContig) {
    bool _pathCreated;
    depth++;

    //If too many nodes have been visited consider creation unsuccessful
    if (depth > 100000) {
        return false;
    }

    _currentNode->hasBeenUsed = true;

    //Check if current node has connections to contigs that haven't been used, and if there is finish path creation
    for (int i = 0; i < _currentNode->backwardsContigConnection.size(); i++) {
        if (_currentNode->backwardsContigConnection.at(i).target->index == _currentContig + 1) {
            pathNodes.insert(pathNodes.begin(), _currentNode->backwardsContigConnection.at(i).target);
            averageSequenceIdentity += _currentNode->backwardsContigConnection.at(i).sequenceIdentity;
            pathNodes.insert(pathNodes.begin(), _currentNode);
            pathLength += _currentNode->backwardsContigConnection.at(i).baseStart;
            return true;
        }
    }

    // Save to variable if all the connections have been used
    bool _allUsed = std::all_of(_currentNode->connections.begin(),
                                _currentNode->connections.end(),
                                [](Connection &connection) { return connection.target->hasBeenUsed; });

    // If the connections vector is empty or all the existing connections were already used in the path return and consider this creation unsuccessful
    if (_currentNode->connections.empty() || _allUsed) {
        return false;
    }

    // Get the sum of all extensions scores in current node's connections
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

        // if the index is out of range continue to generate random number
        if (index > probabilites.size() - 1) {
            continue;
        }

        // Get the picked connection
        Connection connection = probabilites[index].connection;

        // if the picked connection target node has not yet been used, call this method with the target node
        if (!connection.target->hasBeenUsed) {
            _pathCreated = RekurzCreateMonteCarloPath(connection.target, _currentContig);
            // If path was successfully created calculate path length and average sequence identity and add current node to the beginning of the path
            if (_pathCreated) {
                pathNodes.insert(pathNodes.begin(), _currentNode);
                averageSequenceIdentity += connection.sequenceIdentity;
                if (!_currentNode->isContig) {
                    pathLength += connection.baseStart;
                }
                return true;
            }
        } else {
            // If target node has already been used, increase the depth and check if all the connections have already been used
            depth++;

            _allUsed = std::all_of(_currentNode->connections.begin(),
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