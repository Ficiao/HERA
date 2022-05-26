#include "Buckets.h"
#include "Path.h"
#include <vector>

using namespace std;

void Buckets::FillBucketsDeterministic(GraphNode* _readNodes, GraphNode* _contigNodes, int _numberOfContigNodes) {
	Path* _path;
	bool _success;

	for (int i = 0; i < _numberOfContigNodes; i++) {
		for (int j = 0; j < _contigNodes[i].connections.size(); j++) {
			_path = new Path();
			_success = _path->CreateDeterministicPath(_readNodes, &_contigNodes[i], j);
			if (_success == true) {
				int _startContigIndex = _path->pathNodes.front()->index;
				int _endContigIndex = _path->pathNodes.back()->index;

				for (int k = 0; k < buckets.size(); k++) {
					if (buckets.at(k).startContigIndex == _startContigIndex && buckets.at(k).endContigIndex==_endContigIndex) {
						buckets.at(k).paths.push_back(*_path);
						break;
					}
				}

			}

		}
	}


}

void Buckets::FillBucketsMonteCarlo(GraphNode* _readNodes, GraphNode* _contigNodes, int _numberOfContigNodes, int _monteCarloHyperparameter) {

}

Path* Buckets::SelectWinner() {

	return NULL;
}