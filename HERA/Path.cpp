#include <numeric>
#include <map>
#include <random>
#include <algorithm>
#include <stdio.h>
#include "Path.h"
#include "Connection.h"
#include "Range.h"

using namespace std;

bool Path::CreateDeterministicPath(GraphNode *_readNodes, GraphNode *_contigNode, int _indexOfStartingRead) {
    _contigNode->hasBeenUsed = true;

    depth = 0;
    if (RekurzCreateDeterministicPath(_contigNode->connections.at(_indexOfStartingRead).target) == true) {
        pathNodes.insert(pathNodes.begin(), _contigNode);
        averageSequenceIdentity += _contigNode->connections.at(_indexOfStartingRead).sequenceIdentity;
        averageSequenceIdentity = (double) (averageSequenceIdentity / pathNodes.size());

        for (int i = 0; i < pathNodes.size(); i++) {
            pathNodes.at(i)->hasBeenUsed = false;
        }

        return true;
    }

    _contigNode->hasBeenUsed = false;
    return false;
}

bool Path::RekurzCreateDeterministicPath(GraphNode *_currentNode) {
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
            pathLength += _currentNode->connections.at(i).baseStart;
            return true;
        }
    }


    for (int i = 0; i < _currentNode->connections.size() && depth <= 50000; i++) {
        if (_currentNode->connections.at(i).target->hasBeenUsed == false) {
            _pathCreated = RekurzCreateDeterministicPath(_currentNode->connections.at(i).target);
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
//    printf("Generating for contig %d \n", _contigNode->index);
    if (RekurzCreateMonteCarloPath(_contigNode)) {
        pathNodes.insert(pathNodes.begin(), _contigNode);
        averageSequenceIdentity = (double) (averageSequenceIdentity / pathNodes.size());

        for (auto &pathNode: pathNodes) {
            pathNode->hasBeenUsed = false;
        }

        return true;
    }

    _contigNode->hasBeenUsed = false;
    return false;
}

bool Path::RekurzCreateMonteCarloPath(GraphNode *_currentNode) {
    bool _pathCreated;
    depth++;

    if (depth > 5000) {
        return false;
    }

    _currentNode->hasBeenUsed = true;

    for (int i = 0; i < _currentNode->backwardsContigConnection.size(); i++) {
        if (!_currentNode->backwardsContigConnection.at(i).target->hasBeenUsed) {
            pathNodes.insert(pathNodes.begin(), _currentNode->backwardsContigConnection.at(i).target);
            averageSequenceIdentity += _currentNode->backwardsContigConnection.at(i).sequenceIdentity;
            pathNodes.insert(pathNodes.begin(), _currentNode);
            averageSequenceIdentity += _currentNode->connections.at(i).sequenceIdentity;
            pathLength += _currentNode->connections.at(i).baseStart;
            return true;
        }
    }

    // calculate extension score sum
    double extensionScoreSum = std::accumulate(_currentNode->connections.begin(), _currentNode->connections.end(), 0,
                                               [](double i, const Connection &connection) {
                                                   return connection.extensionScore + i;
                                               });

    std::vector<Range> probabilites;
    double start = 0;

    // assign ranges to connections depending on their extension score
    for (auto &connection: _currentNode->connections) {
        double end = start + abs(connection.extensionScore);
        Range range = Range(start, end, connection);
        probabilites.push_back(range);
        start = end;
    }

    while (depth <= 5000) {
        // setup for random number generation
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_real_distribution<double> distr(0, extensionScoreSum); // define the range

        // generate random number
        double random = abs(distr(gen));
//        printf("Generated number %f\n", random);

        // find the right range
        auto it = std::find_if(probabilites.begin(), probabilites.end(), [random](Range range) {
            return range.contains(random);
        });
        long index = std::distance(probabilites.begin(), it);

        Connection connection = probabilites[index].connection;

        if (index >= probabilites.size() - 1) {
            return false;
        }

        if (!connection.target->hasBeenUsed) {
            _pathCreated = RekurzCreateMonteCarloPath(connection.target);
            if (_pathCreated) {
                pathNodes.insert(pathNodes.begin(), _currentNode);
                averageSequenceIdentity += connection.sequenceIdentity;
                pathLength += connection.baseStart;
                return true;
            }
        } else {
            depth++;
        }
    }

    _currentNode->hasBeenUsed = false;
    return false;
}