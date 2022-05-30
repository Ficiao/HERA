#include "DataLoader.h"
#include "GraphNode.h"
#include "ConnectionsSorter.h"
#include "Buckets.h"
#include "FinalContigConstructor.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int startTime = time(nullptr);

    if (argc != 8) {
        printf("Please enter right amount of arguments\n");
        printf("1 - PAF file with reads overlaps, including reversible complements\n");
        printf("2 - PAF file with contigs and reads overlaps, including reads reversible complements\n");
        printf("3 - Number of read nodes\n");
        printf("4 - Number of contigs\n");
        printf("5 - FASTA file for reads genome sequences including reversible complements\n");
        printf("6 - FASTA file for contig genome sequences\n");
        printf("7 - Name of the file in which the result will be written\n");

        return 0;
    }

    std::string _readsOverlapsFile = argv[1];
    std::string _contigsOverlapsFile = argv[2];
    int _readNodesNumber = atoi(argv[3]);
    int _contigNodesNumber = atoi(argv[4]);
    std::string _fastaReadsFile = argv[5];
    std::string _fastaContigsFile = argv[6];
    std::string _resultFile = argv[7];

    DataLoader *_dataLoader = new DataLoader();
    GraphNode *_readNodes = _dataLoader->LoadData(_readsOverlapsFile, NULL, false, _readNodesNumber);
    GraphNode *_contigNodes = _dataLoader->LoadData(_contigsOverlapsFile, _readNodes, true,
                                                    _contigNodesNumber);
    printf("\n");

    Buckets *_buckets = new Buckets(_contigNodesNumber);
    ConnectionsSorter *_connectionsSorter = new ConnectionsSorter();

    _connectionsSorter->sortConnectionsByOverlapScore(_readNodes, _readNodesNumber);
    _buckets->FillBucketsDeterministic(_contigNodes, _contigNodesNumber);

    _connectionsSorter->sortConnectionsByExtensionScore(_readNodes, _readNodesNumber);
    _buckets->FillBucketsDeterministic(_contigNodes, _contigNodesNumber);
    printf("\n");

    _buckets->FillBucketsMonteCarlo(_contigNodes, _contigNodesNumber);
    printf("\n");

    std::vector<Path> _paths = _buckets->SelectWinner(_contigNodesNumber);
    printf("\n");

    FinalContigConstructor *finalContigConstructor = new FinalContigConstructor();
    finalContigConstructor->construct(_paths, _fastaReadsFile,
                                      _fastaContigsFile, _resultFile);
    printf("\n");

    int endTime = time(nullptr);
    double executionTime = (endTime - startTime) % 60;
    printf("Execution time: %.2f s", executionTime);
}