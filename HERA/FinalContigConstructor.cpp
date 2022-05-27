#include "FinalContigConstructor.h"
#include "DataLoader.h"
#include "Connection.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void
FinalContigConstructor::construct(std::vector<Path> paths, std::string readsFilename,
                                  std::string contigsFilename, std::string resultFilename) {
    loadFile(readsFilename);
    loadFile(contigsFilename);

    std::vector<GraphNode *> finalPath;

    std::ofstream _fin;
    std::string file = resultFilename + "-" + std::to_string(time(nullptr)) + ".fasta";
    _fin.open(file);

    printf("Writing to file: %s\n", file.c_str());

    _fin << ">BIOINFORMATIKA Escherichia coli genome\n";

    for (int i = 0; i < paths.size(); ++i) {
        Path path = paths[i];
        if (i < paths.size() - 1) {
            finalPath.insert(finalPath.end(), path.pathNodes.begin(), path.pathNodes.end() - 1);
        } else {
            finalPath.insert(finalPath.end(), path.pathNodes.begin(), path.pathNodes.end());
        }
    }

    Connection leftConnection{};
    Connection rightConnection{};
    for (int i = 0; i < finalPath.size(); ++i) {
        GraphNode *currentNode = finalPath[i];
        int currentIndex = currentNode->index;

        // popuni next node i konekciju ako nije zadnji node
        if (i < finalPath.size() - 1) {
            GraphNode *nextNode = finalPath[i + 1];
            if (nextNode->isContig) {
                rightConnection = findConnectionWithNode(nextNode->index, currentNode->backwardsContigConnection);
            } else {
                rightConnection = findConnectionWithNode(nextNode->index, currentNode->connections);
            }
        }

        // ako nije prvi node popuni leftConnection
        if (i != 0) {
            GraphNode *previousNode = finalPath[i - 1];
            leftConnection = findConnectionWithNode(currentIndex, previousNode->connections);
        }

        std::string read;
        // popuni read
        if (currentNode->isContig) {
            read = contigs[currentIndex];
        } else {
            read = reads[currentIndex];
        }

//        if (currentNode->isContig) {
//            _fin << "\n";
//            continue;
//        }
        if (i == 0) {
            _fin << read.substr(0, rightConnection.baseEnd);
        } else if (i < finalPath.size() - 1) {
            _fin << read.substr(leftConnection.targetEnd, rightConnection.baseEnd);
        } else if (i == finalPath.size() - 1) {
            _fin << read.substr(leftConnection.targetEnd, read.size());
        }
    }

    _fin.close();

    printf("Wrote path to file: %s\n", file.c_str());
}

void FinalContigConstructor::loadFile(std::string filename) {
    int _lineCount = 0;
    std::ifstream _fin;
    std::string _line;
    _fin.open(filename);
    int indexFromFile = 0;
    DataLoader *dataLoader = new DataLoader();

    printf("Reading file: %s\n", filename.c_str());

    while (getline(_fin, _line)) {
        _lineCount++;

        if (_lineCount % 1000 == 0) {
            printf("Lines read: %d\n", _lineCount);
        }

        if (_line.rfind(">read", 0) == 0) {
            indexFromFile = dataLoader->GetIndexFromName(_line);

            if (getline(_fin, _line)) {
                _lineCount++;
                reads[indexFromFile] = _line;
            }
        } else if (_line.rfind(">ctg", 0) == 0) {
            indexFromFile = dataLoader->GetIndexFromName(_line);

            if (getline(_fin, _line)) {
                _lineCount++;
                contigs[indexFromFile] = _line;
            }
        } else {
            continue;
        }
    }

    _fin.close();
}

Connection FinalContigConstructor::findConnectionWithNode(int i, std::vector<Connection> connections) const {
    auto it = std::find_if(connections.begin(),
                           connections.end(),
                           [i](const Connection &connection) {
                               return connection.target->index == i;
                           });
    long index = std::distance(connections.begin(), it);

    return connections[index];
}
