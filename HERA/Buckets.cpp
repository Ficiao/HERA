#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Buckets.h"
#include "Connection.h"

using namespace std;

void Buckets::FillBucketsDeterministic(GraphNode* _readNodes, GraphNode* _contigNodes, int _numberOfContigNodes) {
	Path* _path;
	bool _success;

	//za svaku contigu, za svaki direktni overlap koji taj contiga ima sa readovima pokusaj sloziti put
	for (int i = 1; i <= _numberOfContigNodes; i++) {

		if (i <= _numberOfContigNodes / 2) {
			_contigNodes[i + (_numberOfContigNodes / 2)].hasBeenUsed = true;
		}
		else {
			_contigNodes[i - (_numberOfContigNodes / 2)].hasBeenUsed = true;
		}

		for (int j = 0; j < _contigNodes[i].connections.size(); j++) {
			_path = new Path();
			_success = _path->CreateDeterministicPath(_readNodes, &_contigNodes[i], j);

			//ako je put uspjesno slozen, otkrij u koji bucket ovisno o pocetnoj i zavrsnoj contigi put pripada i stavi ga tamo
			if (_success == true) {
				if (_path->averageSequenceIdentity > 0.85f) {
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

		if (i <= _numberOfContigNodes / 2) {
			_contigNodes[i + (_numberOfContigNodes / 2)].hasBeenUsed = false;
		}
		else {
			_contigNodes[i - (_numberOfContigNodes / 2)].hasBeenUsed = false;
		}

	}


}

void Buckets::FillBucketsMonteCarlo(GraphNode* _readNodes, GraphNode* _contigNodes, int _numberOfContigNodes, int _monteCarloHyperparameter) {

}

void Buckets::SelectWinner() {
    // mozda koristit novi random generator
    srand(time(nullptr));

    for (auto & bucket : buckets) {
        int num = abs(rand()) % bucket.paths.size();
        bucket.winningPathIndex = num;
        printf("For bucket %d %d winner path is with index %d\n", bucket.startContigIndex, bucket.endContigIndex, bucket.winningPathIndex);
    }
}