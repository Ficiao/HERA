#include "DataLoader.h"


int main() {

	DataLoader* _dataLoader = new DataLoader();
	_dataLoader->LoadData("overlap_reads_ava_pb.paf");
	_dataLoader->LoadData("overlaps_reads_contigs_ava_pb.paf");

}