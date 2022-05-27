#include "FinalContigConstructor.h"
#include "DataLoader.h"
#include "Connection.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void
FinalContigConstructor::construct(std::vector<Bucket> buckets, std::string readsFilename, std::string contigsFilename,
                                  std::string resultFilename) {
    // todo treba stavit da nije 3 hardkodirano
    int numberOfContigs = 3;
    for (int i = 1; i <= numberOfContigs; ++i) {
        auto it = std::find_if(buckets.begin(),
                               buckets.end(),
                               [i, numberOfContigs](const Bucket &bucket) {
                                   return bucket.startContigIndex == i && bucket.endContigIndex == (i < numberOfContigs ? i + 1 : 1);
                               });
        // TODO zbog ovoga mozda treba dodat zadnji bucket da bude prazni, trenutno ne radi probleme
        if (it != buckets.end()) {
            long index = std::distance(buckets.begin(), it);
            addToFullPath(buckets[index]);
        }
    }
    // potrebno zbog pretrazivanja poslije
    finalPath.pathNodes.insert(finalPath.pathNodes.end(), new GraphNode());


    getReadings(readsFilename);
    getReadings(contigsFilename);

    writeToFile(resultFilename);
}

void FinalContigConstructor::writeToFile(std::string filename) {
    std::ofstream _fin;
    std::string file = filename + std::to_string(time(nullptr)) + ".fasta";
    _fin.open(file);

    printf("Writing to file: %s\n", file.c_str());

    // -2 = -1 za prazan zadnji node i -1 za skipanje zadnjeg nodea
    for (std::size_t i = 0; i < finalPath.pathNodes.size() - 2; ++i) {
        auto currentNode = finalPath.pathNodes[i];
        auto nextNode = finalPath.pathNodes[i + 1];

        if (currentNode->index == nextNode->index) {
            continue;
        }

        auto connection = findConnectionWithNode(nextNode->index, currentNode->connections);


        printf("Current Node %d\n", currentNode->index);
        printf("Next Node %d\n", nextNode->index);
        printf("Connection (base start=%d, base end=%d) (target start=%d target end =%d)\n", connection.baseStart,
               connection.baseEnd, connection.targetStart, connection.targetEnd);

        if (i == 0) {
//            _fin << currentNode->read.substr(0, connection.baseEnd);
        } else if (i < finalPath.pathNodes.size() - 3) {
//            _fin << currentNode->read.substr(connection.baseStart, connection.baseEnd);
        } else if (i == finalPath.pathNodes.size() - 3) {
//            _fin << currentNode->read.substr(connection.baseStart, connection.baseEnd);
//            _fin << nextNode->read.substr(connection.targetStart, connection.targetEnd);
        }
        _fin << currentNode->read;

    }
    _fin.close();
}

void FinalContigConstructor::addToFullPath(Bucket bucket) {
    Path path = bucket.paths[bucket.winningPathIndex];

    finalPath.pathNodes.insert(finalPath.pathNodes.end(), path.pathNodes.begin(), path.pathNodes.end());
}

void FinalContigConstructor::getReadings(std::string filename) {
    int _lineCount = 0;
    std::ifstream _fin;
    std::string _line;
    _fin.open(filename);
    int indexFromFile = 0;
    DataLoader *dataLoader = new DataLoader();

    printf("Reading file: %s\n", filename.c_str());

    while (getline(_fin, _line)) {
        _lineCount++;
        GraphNode *node;

        if (_lineCount % 100 == 0) {
            printf("Lines read: %d\n", _lineCount);
        }

        if (_line.rfind(">read", 0) == 0) {
            indexFromFile = dataLoader->GetIndexFromName(_line);
            node = findNodeWithIndex(indexFromFile, false);

            if (node->index != 0 && getline(_fin, _line)) {
                _lineCount++;
                node->read = _line;
            }
        } else if (_line.rfind(">ctg", 0) == 0) {
            indexFromFile = dataLoader->GetIndexFromName(_line);
            node = findNodeWithIndex(indexFromFile, true);

            if (node->index != 0 && getline(_fin, _line)) {
                _lineCount++;
                node->read = _line;
            }
        } else {
            continue;
        }
    }

    _fin.close();
}

GraphNode *FinalContigConstructor::findNodeWithIndex(int i, bool contig) const {
    std::vector<GraphNode *> nodes = finalPath.pathNodes;
    auto it = std::find_if(nodes.begin(),
                           nodes.end(),
                           [i, contig](const GraphNode *node) {
                               if (contig) {
                                   printf("IT'S A CONTIG");
                               }
//                                printf("full %s, is contig %s\n", (node->index == i and node->contig == contig) ? "true" : "false", contig ? "true" : "false");
                               return (node->index == i &&
                                       ((node->isContig && contig) || (!node->isContig && !contig)));
                           });
    long index = std::distance(nodes.begin(), it);

    return nodes[index];
}

Connection
FinalContigConstructor::findConnectionWithNode(int i, std::vector<Connection> connections) const {
    auto it = std::find_if(connections.begin(),
                           connections.end(),
                           [i](const Connection &connection) {
                               return connection.target->index == i;
                           });
    long index = std::distance(connections.begin(), it);

    return connections[index];
}