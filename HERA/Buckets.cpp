#include <stdio.h>
#include <algorithm>
#include "Buckets.h"
#include "Connection.h"

using namespace std;

void Buckets::FillBucketsDeterministic(GraphNode *_readNodes, GraphNode *_contigNodes, int _numberOfContigNodes) {
    Path *_path;
    bool _success;

    //za svaku contigu, za svaki direktni overlap koji taj contiga ima sa readovima pokusaj sloziti put
    for (int i = 1; i <= _numberOfContigNodes; i++) {

        for (int j = 0; j < _contigNodes[i].connections.size(); j++) {
            _path = new Path();
            _success = _path->CreateDeterministicPath(_readNodes, &_contigNodes[i], j);

            //ako je put uspjesno slozen, otkrij u koji bucket ovisno o pocetnoj i zavrsnoj contigi put pripada i stavi ga tamo
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
}

void Buckets::FillBucketsMonteCarlo(GraphNode* _readNodes, GraphNode* _contigNodes, int _numberOfContigNodes) {
	int _numberOfDeterministicPaths = 0;

	for (int i = 0; i < buckets.size(); i++) {
		_numberOfDeterministicPaths += buckets.at(i).paths.size();
	}

	int _numberOfStochasticPaths = 0;
	Path* _path;
	bool _success;

	while (_numberOfStochasticPaths < _numberOfDeterministicPaths) {
//	    printf("Iterating through contigs\n");

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
						if (buckets.at(k).startContigIndex == _startContigIndex && buckets.at(k).endContigIndex == _endContigIndex) {
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

        auto pathIt = std::max_element(winninContigBucket.paths.begin(),
                                       winninContigBucket.paths.end(),
                                       [](const Path &left, const Path &right) {
                                           return left.pathLength < right.pathLength;
                                       });

        long pathIndex = std::distance(winninContigBucket.paths.begin(), pathIt);
        Path winningBucketPath = winninContigBucket.paths[pathIndex];

        printf("For bucket %d %d winner path is with pathIndex %ld and length %d\n",
               winninContigBucket.startContigIndex,
               winninContigBucket.endContigIndex,
               pathIndex,
               winningBucketPath.pathLength);

        winningPaths.push_back(winningBucketPath);
    }
    return winningPaths;
}