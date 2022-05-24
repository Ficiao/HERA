#include "DataLoader.h"
#include "GraphNode.h"

int main() {

	DataLoader* _dataLoader = new DataLoader();
	GraphNode* _readNodes = _dataLoader->LoadData("overlap_reads_ava_pb.paf", NULL, false);
	GraphNode* _contigNodes = _dataLoader->LoadData("overlaps_reads_contigs_ava_pb.paf", _readNodes, true);

}