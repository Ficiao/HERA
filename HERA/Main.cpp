#include "DataLoader.h"
#include "GraphNode.h"
#include "ConnectionsSorter.h"
#include "Connection.h"

int main() {
    int nodesSize = 9284;

    DataLoader* _dataLoader = new DataLoader();
    GraphNode* _readNodes = _dataLoader->LoadData("overlap_reads_rc_ava_pb.paf", NULL, false, nodesSize);
	GraphNode* _contigNodes = _dataLoader->LoadData("overlaps_reads_contigs_rc_ava_pb.paf", _readNodes, true, 6);

    GraphNode* _testNodes = new GraphNode[2];
    GraphNode* _oneNode = &_testNodes[0];
    GraphNode* _twoNode = &_testNodes[1];

    Connection* _connectionOne = new Connection();
    Connection* _connectionTwo = new Connection();
    _connectionOne->overlapScore = 10;
    _connectionTwo->overlapScore = 5;

    _twoNode->connections.push_back(*_connectionTwo);
    _twoNode->connections.push_back(*_connectionOne);

    ConnectionsSorter* _connectionsSorter = new ConnectionsSorter();
    _connectionsSorter->sortConnectionsByOverlapScore(_testNodes, 2);
    printf("Done");
}