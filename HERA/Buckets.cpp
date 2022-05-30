#include <stdio.h>
#include <algorithm>
#include <numeric>
#include "Buckets.h"
#include "Connection.h"

using namespace std;

//Add paths to buckets using greedy algorithm. Expects node connections to be sorted based on looked value.
void Buckets::FillBucketsDeterministic(GraphNode *_readNodes, GraphNode *_contigNodes, int _numberOfContigNodes) {
    Path *_path;
    bool _success;

    //For each contig, for each direct overlap that contig has with read nodes attempt to create a path
    for (int i = 1; i <= _numberOfContigNodes; i++) {

        for (int j = 0; j < _contigNodes[i].connections.size(); j++) {
            _path = new Path();
            _success = _path->CreateDeterministicPath(_readNodes, &_contigNodes[i], j);

            //if path is successfully created, figure out which bucked it belongs to based on start and end path index
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

//TODO: comment
void Buckets::FillBucketsMonteCarlo(GraphNode *_readNodes, GraphNode *_contigNodes, int _numberOfContigNodes) {
    int _numberOfDeterministicPaths = 0;

    for (int i = 0; i < buckets.size(); i++) {
        _numberOfDeterministicPaths += buckets.at(i).paths.size();
    }

    int _numberOfStochasticPaths = 0;
    Path *_path;
    bool _success;

    int rotation = 0;

    while (_numberOfStochasticPaths < _numberOfDeterministicPaths) {
//	    printf("Iterating through contigs\n");

        if (rotation % 10 == 0) {
            printf("Generated %d / %d Monte Carlo paths\n", _numberOfStochasticPaths, _numberOfDeterministicPaths);
        }

        rotation++;
        for (int i = 1; i <= _numberOfContigNodes; i++) {
            _path = new Path();
            _success = _path->CreateMonteCarloPath(_readNodes, &_contigNodes[i]);

            //ako je put uspjesno slozen, otkrij u koji bucket ovisno o pocetnoj i zavrsnoj contigi put pripada i stavi ga tamo
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

//TODO: comment
std::vector<Path> Buckets::SelectWinner(int numberOfContigs) {
    std::vector<Path> winningPaths;

    for (int i = 1; i < numberOfContigs; ++i) {
        std::vector<Bucket> contigBuckets;

        for (auto &bucket: buckets) {
            if (bucket.startContigIndex == i && bucket.endContigIndex == i + 1) {
                contigBuckets.push_back(bucket);
            }
        }

        auto bucketIt = std::max_element(contigBuckets.begin(),
                                         contigBuckets.end(),
                                         [](const Bucket &left, const Bucket &right) {
                                             return left.paths.size() < right.paths.size();
                                         });

        long bucketIndex = std::distance(contigBuckets.begin(), bucketIt);
        Bucket winninContigBucket = contigBuckets[bucketIndex];

        auto avgPath =
                std::accumulate(winninContigBucket.paths.begin(), winninContigBucket.paths.end(), 0,
                                [](int i, Path &path) {
                                    return path.pathLength + i;
                                }) / winninContigBucket.paths.size();

        std::sort(winninContigBucket.paths.begin(),
                  winninContigBucket.paths.end(),
                  [](const Path &left, const Path &right) {
                      return left.pathLength < right.pathLength;
                  });

        printf("Bucket %d %d average path %lu\n", winninContigBucket.startContigIndex,
               winninContigBucket.endContigIndex, avgPath);

        std::vector<Path> pathsCopy = winninContigBucket.paths;

        int n = int(winninContigBucket.paths.size() / 10);

        std::nth_element(pathsCopy.begin(), pathsCopy.begin() + n - 1, pathsCopy.end(), [](Path &left, Path &right) {
            return left.pathLength > right.pathLength;
        });

        Path winningBucketPath = *(pathsCopy.begin() + n - 1);

        printf("For bucket %d %d winner path is with and length %d\n",
               winninContigBucket.startContigIndex,
               winninContigBucket.endContigIndex,
//               pathIndex,
               winningBucketPath.pathLength);

        winningPaths.push_back(winningBucketPath);
    }
    return winningPaths;
}