#include "DataLoader.h"
#include "GraphNode.h"

int main(char* argv[]) {

	DataLoader* _dataLoader = new DataLoader();
	GraphNode* _readNodes = _dataLoader->LoadData("overlap_reads_rc_ava_pb.paf", NULL, false, 9284);
	GraphNode* _contigNodes = _dataLoader->LoadData("overlaps_reads_contigs_rc_ava_pb.paf", _readNodes, true, 6);

}