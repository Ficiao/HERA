#include "DataLoader.h"
#include "GraphNode.h"
#include "ConnectionsSorter.h"
#include "Buckets.h"
#include "FinalContigConstructor.h"

int main() {
    int _readNodesNumber = 9284;
    int _contigNodesNumber = 6;
    int _monteCarloHyperparameter = 4;

    DataLoader* _dataLoader = new DataLoader();
    GraphNode* _readNodes = _dataLoader->LoadData("overlap_reads_rc_ava_pb.paf", NULL, false, _readNodesNumber);
	GraphNode* _contigNodes = _dataLoader->LoadData("overlaps_reads_contigs_rc_ava_pb.paf", _readNodes, true, _contigNodesNumber);

    Buckets* _buckets = new Buckets(_contigNodesNumber);
    ConnectionsSorter* _connectionsSorter = new ConnectionsSorter();

    _connectionsSorter->sortConnectionsByOverlapScore(_readNodes, _readNodesNumber);
    _buckets->FillBucketsDeterministic(_readNodes, _contigNodes, _contigNodesNumber);

    _connectionsSorter->sortConnectionsByExtensionScore(_readNodes, _readNodesNumber);
    _buckets->FillBucketsDeterministic(_readNodes, _contigNodes, _contigNodesNumber);

    //_buckets->FillBucketsMonteCarlo(_readNodes, _contigNodes, _contigNodesNumber, _monteCarloHyperparameter);
    
    _buckets->SelectWinner();

    FinalContigConstructor* finalContigConstructor = new FinalContigConstructor();
    finalContigConstructor->construct(_buckets->buckets, "ecoli_test_reads_with_complements.fasta", "ecoli_test_contigs_with_complements.fasta", "result");

}