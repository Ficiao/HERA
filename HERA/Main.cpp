#include "DataLoader.h"
#include "GraphNode.h"
#include "ConnectionsSorter.h"

int main() {
    int nodesSize = 9284;

    DataLoader* _dataLoader = new DataLoader();
    GraphNode* _readNodes = _dataLoader->LoadData("overlap_reads_rc_ava_pb.paf", NULL, false, nodesSize);
	GraphNode* _contigNodes = _dataLoader->LoadData("overlaps_reads_contigs_rc_ava_pb.paf", _readNodes, true, 6);

    ConnectionsSorter* _connectionsSorter = new ConnectionsSorter();
    _connectionsSorter->sortConnectionsByOverlapScore(_readNodes, nodesSize);
}