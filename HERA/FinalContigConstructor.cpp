// @Author Fribert

#include "FinalContigConstructor.h"
#include "DataLoader.h"
#include "Connection.h"
#include <algorithm>
#include <fstream>
#include <iostream>

// Function for constructing the final sequence
// Needs to have a list of winning paths, files to read the parts of the genome from and the name of a file to write the result to
void
FinalContigConstructor::construct(std::vector<Path> paths, std::string readsFilename,
                                  std::string contigsFilename, std::string resultFilename) {
    // Load nodes genome sequences
    loadFile(readsFilename, false);
    loadFile(contigsFilename, true);

    std::vector<GraphNode *> finalPath;

    std::ofstream _fin;
    std::string file = resultFilename + "-" + std::to_string(time(nullptr)) + ".fasta";
    _fin.open(file);

    _fin << ">BIOINFORMATIKA Escherichia coli genome\n";

    // Go through winning paths and collect them to one single vector without the joint nodes between the paths
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
    // Go through all nodes in order of the final path
    for (int i = 0; i < finalPath.size(); ++i) {
        GraphNode *currentNode = finalPath[i];
        int currentIndex = currentNode->index;

        // Get the next node and right connection if the current node is not at the end of the final path
        if (i < finalPath.size() - 1 ) {
            GraphNode *nextNode = finalPath[i + 1];
            if (nextNode->isContig && !currentNode->backwardsContigConnection.empty()) {
                rightConnection = findConnectionWithNode(nextNode->index, currentNode->backwardsContigConnection);
            } else if (!currentNode->connections.empty()){
                rightConnection = findConnectionWithNode(nextNode->index, currentNode->connections);
            }
        }

        // If the current node is not the first one in the final path get the left connection with the previous node
        if (i != 0 && !currentNode->connections.empty()) {
            GraphNode *previousNode = finalPath[i - 1];
            leftConnection = findConnectionWithNode(currentIndex, previousNode->connections);
        }

        std::string read;
        // Fill the current node with genome read that belongs to it
        if (currentNode->isContig) {
            read = contigs[currentIndex];
        } else {
            read = reads[currentIndex];
        }

        if (i == 0) {
            // First node of final path
            _fin << read.substr(0, rightConnection.baseEnd);
        } else if (i < finalPath.size() - 1) {
            // Middle nodes of the final path
            _fin << read.substr(leftConnection.targetEnd, rightConnection.baseEnd);
        } else if (i == finalPath.size() - 1) {
            // Last node of the final path
            _fin << read.substr(leftConnection.targetEnd, read.size());
        }
    }

    _fin.close();

    printf("Wrote path to file: %s\n", file.c_str());
}

// Function for loading the genome reads from a file, needs to know if it is reading contig file or read file
void FinalContigConstructor::loadFile(std::string filename, bool isContigFile) {
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

        if (_line.rfind('>', 0) == 0 && !isContigFile) {
            indexFromFile = dataLoader->GetIndexFromName(_line);

            if (getline(_fin, _line)) {
                _lineCount++;
                reads[indexFromFile] = _line;
            }
        } else if (_line.rfind('>', 0) == 0 && isContigFile) {
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

// Function for finding a connection that has a target node index i in list of connections
Connection FinalContigConstructor::findConnectionWithNode(int i, std::vector<Connection> connections) const {
    auto it = std::find_if(connections.begin(),
                           connections.end(),
                           [i](const Connection &connection) {
                               return connection.target->index == i;
                           });
    long index = std::distance(connections.begin(), it);

    return connections[index];
}
