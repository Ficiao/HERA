#include <stdio.h>
#include <algorithm>
#include <numeric>
#include "Buckets.h"
#include "Connection.h"

using namespace std;

//Add paths to buckets using greedy algorithm. Expects node connections to be sorted based on looked value.
void Buckets::FillBucketsDeterministic(GraphNode *_contigNodes, int _numberOfContigNodes) {
    Path *_path;
    bool _success;

    //For each contig, for each direct overlap that contig has with read nodes attempt to create a path
    for (int i = 1; i <= _numberOfContigNodes; i++) {

        for (int j = 0; j < _contigNodes[i].connections.size(); j++) {
            _path = new Path();
            _success = _path->CreateDeterministicPath(&_contigNodes[i], j);

            //if path is successfully created, figure out which bucket it belongs to based on start and end path index
            if (_success == true) {
                if (_path->averageSequenceIdentity > 0.9f) {
                    int _startContigIndex = _path->pathNodes.front()->index;
                    int _endContigIndex = _path->pathNodes.back()->index;

                    for (int k = 0; k < buckets.size(); k++) {
                        if (buckets.at(k).startContigIndex == _startContigIndex &&
                            buckets.at(k).endContigIndex == _endContigIndex) {
                            buckets.at(k).paths.push_back(*_path);
                            break;
                        }
                    }
                }
            }
        }
    }
    printf("Generated Deterministic paths\n");
}

void Buckets::FillBucketsMonteCarlo(GraphNode *_contigNodes, int _numberOfContigNodes) {
    int _numberOfDeterministicPaths = 0;

    // Collect the number of paths made using the deterministic algorithms
    for (int i = 0; i < buckets.size(); i++) {
        _numberOfDeterministicPaths += buckets.at(i).paths.size();
    }

    int _numberOfStochasticPaths = 0;
    Path *_path;
    bool _success;

    // Search for paths until more stohastic paths are made than deterministic paths
    while (_numberOfStochasticPaths < _numberOfDeterministicPaths) {

        if (_numberOfStochasticPaths % 10 == 0) {
            printf("Generated %d / %d Monte Carlo paths\n", _numberOfStochasticPaths, _numberOfDeterministicPaths);
        }

        // For each contig try to generate a path
        for (int i = 1; i <= _numberOfContigNodes; i++) {
            _path = new Path();
            _success = _path->CreateMonteCarloPath(&_contigNodes[i]);

            //if path is successfully created, figure out which bucket it belongs to based on start and end path index
            if (_success == true) {
                if (_path->averageSequenceIdentity >= 0.5f) {
                    _numberOfStochasticPaths++;
                    int _startContigIndex = _path->pathNodes.front()->index;
                    int _endContigIndex = _path->pathNodes.back()->index;

                    for (int k = 0; k < buckets.size(); k++) {
                        if (buckets.at(k).startContigIndex == _startContigIndex &&
                            buckets.at(k).endContigIndex == _endContigIndex) {
                            buckets.at(k).paths.push_back(*_path);
                            break;
                        }
                    }
                }
            }

        }

    }

}

std::vector<Path> Buckets::SelectWinner(int numberOfContigs) {
    std::vector<Path> winningPaths;

    // for each contig
    for (int i = 1; i < numberOfContigs; ++i) {
        std::vector<Bucket> contigBuckets;

        // find buckets whose startContigIndex is the same as i
        for (auto &bucket: buckets) {
            if (bucket.startContigIndex == i && bucket.endContigIndex == i + 1) {
                contigBuckets.push_back(bucket);
            }
        }

        // find the bucket with the most paths
        auto bucketIt = std::max_element(contigBuckets.begin(),
                                         contigBuckets.end(),
                                         [](const Bucket &left, const Bucket &right) {
                                             return left.paths.size() < right.paths.size();
                                         });

        long bucketIndex = std::distance(contigBuckets.begin(), bucketIt);
        Bucket winninContigBucket = contigBuckets[bucketIndex];

        // calculate average path in winning bucket
        auto avgPath =
                std::accumulate(winninContigBucket.paths.begin(), winninContigBucket.paths.end(), 0,
                                [](int i, Path &path) {
                                    return path.pathLength + i;
                                }) / winninContigBucket.paths.size();

        // sort paths in winning bucket by their length
        std::sort(winninContigBucket.paths.begin(),
                  winninContigBucket.paths.end(),
                  [](const Path &left, const Path &right) {
                      return left.pathLength < right.pathLength;
                  });

        printf("Bucket %d %d average path %lu\n", winninContigBucket.startContigIndex,
               winninContigBucket.endContigIndex, avgPath);

        std::vector<Path> pathsCopy = winninContigBucket.paths;

        // n will be index for finding the winning path
        // n represents the path that is the first one after the top 10% of paths with longest paths
        int n = int(winninContigBucket.paths.size() / 10);

        // get the path that is in place n in the sorted path vector
        std::nth_element(pathsCopy.begin(), pathsCopy.begin() + n - 1, pathsCopy.end(), [](Path &left, Path &right) {
            return left.pathLength > right.pathLength;
        });

        Path winningBucketPath = *(pathsCopy.begin() + n - 1);

        printf("For bucket %d %d winner path with length %d\n",
               winninContigBucket.startContigIndex,
               winninContigBucket.endContigIndex,
               winningBucketPath.pathLength);

        // collect the winning paths
        winningPaths.push_back(winningBucketPath);
    }

    return winningPaths;
}