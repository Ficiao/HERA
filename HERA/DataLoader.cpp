#include <string>
#include <fstream>
#include "DataLoader.h"
#include <iostream>
#include <vector>
#include "ContigNode.h"
#include "Connection.h"
#include "ReadNode.h"

using namespace std;

GraphNode* DataLoader::LoadData(string _fileName, GraphNode* _targetNodes) {

    int _numberOfElems = IndexOfLastElem(_fileName);
    _numberOfElems++;

    GraphNode* _nodes = new GraphNode[_numberOfElems];
    if (_targetNodes != NULL) {
        FillNodes(_nodes, _targetNodes, _fileName);
    }
    else {
        FillNodes(_nodes, _nodes, _fileName);
    }

    return _nodes;
}

void DataLoader::FillNodes(GraphNode* _nodes, GraphNode* _targetNodes, string _fileName) {
    int _lineCount = 0;
    ifstream _fin;
    string _line;
    string _delimiter = "\t";
    _fin.open(_fileName);
    int index;
    GraphNode* _baseNode;
    GraphNode* _targetNode;
    Connection* _connection;

    while (getline(_fin, _line)) {
        _lineCount++;
        if (_lineCount % 1000==0) {
            printf("%d\n",_lineCount);
        }
        vector<string> words{};

        size_t pos = 0;
        for (int i = 0; i < 11; i++) {
            pos = _line.find(_delimiter);
            words.push_back(_line.substr(0, pos));
            _line.erase(0, pos + _delimiter.length());
        }
        
        index = GetIndexFromName(words[0]);

        _baseNode = &_nodes[index];
        _baseNode->index = index;
        _baseNode->size = stoi(words[1]);

        index = GetIndexFromName(words[5]);
        _targetNode = &_targetNodes[index];
        _connection = new Connection();
        _connection->_base = _baseNode;
        _connection->_baseStart = stoi(words[2]);
        _connection->_baseEnd = stoi(words[3]);
        _connection->_relativeStrand = words[4].at(0);
        _connection->_target = _targetNode;
        _connection->_targetStart = stoi(words[7]);
        _connection->_targetEnd = stoi(words[8]);
        _connection->_residueMatches = stoi(words[9]);
        _connection->_allignmentBlockLenth = stoi(words[10]);

        _baseNode->connections.push_back(*_connection);

    }   
}

int DataLoader::IndexOfLastElem(std::string _fileName)
{
    ifstream _fin;
    string _nameOfLast;

    _fin.open(_fileName);
    if (_fin.is_open()) {
        _fin.seekg(-2, ios_base::end);                // go to one spot before the EOF

        bool _keepLooping = true;
        while (_keepLooping) {
            char _character;
            _fin.get(_character);                            // Get current byte's data

            if ((int)_fin.tellg() <= 1) {             // If the data was at or before the 0th byte
                _fin.seekg(0);                       // The first line is the last line
                _keepLooping = false;                // So stop there
            }
            else if (_character == '\n') {                   // If the data was a newline
                _keepLooping = false;                // Stop at the current position.
            }
            else {                                  // If the data was neither a newline nor at the 0 byte
                _fin.seekg(-2, ios_base::cur);        // Move to the front of that data, then to the front of the data before it
            }
        }

        string _lastLine;
        string _lastLinePom;
        getline(_fin, _lastLine);

        while (getline(_fin, _lastLinePom)) {
            _lastLine = _lastLinePom;
        }
        
        _nameOfLast = _lastLine.substr(0, _lastLine.find("\t"));        
        _fin.close();
    }

    return GetIndexFromName(_nameOfLast);
}

int DataLoader::GetIndexFromName(string _name) {
    int _start = -1;
    int _end = -1;

    for (int i = 0; i < _name.length(); i++) {
        if (isdigit(_name[i])) {
            if (_start == -1) {
                _start = i;
            }
        }
        else if (_start != -1) {
            _end = i - 1;
            break;
        }
    }
    if (_end == -1) {
        _end = _name.length() - 1;
    }

    _name = _name.substr(_start, _end);
    return stoi(_name);
}
