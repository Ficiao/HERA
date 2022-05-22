#include "DataLoader.h"
#include "ReadNode.h"
#include "ContigNode.h"

int main() {

	DataLoader* _dataLoader = new DataLoader();
	ReadNode* _readNodes = (ReadNode*)_dataLoader->LoadData("overlap_reads_ava_pb.paf", NULL);
	ContigNode* _contigNodes = (ContigNode*)_dataLoader->LoadData("overlaps_reads_contigs_ava_pb.paf", _readNodes);

}