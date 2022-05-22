#include <string>
#include <fstream>
#include "DataLoader.h"
#include <iostream>
#include <vector>

using namespace std;

void DataLoader::LoadData(string _fileName) {

    int _numberOfElems = IndexOfLastElem(_fileName);


}

int DataLoader::IndexOfLastElem(std::string& _fileName)
{
    ifstream fin;
    string _nameOfLast;

    fin.open(_fileName);
    if (fin.is_open()) {
        fin.seekg(-2, ios_base::end);                // go to one spot before the EOF

        bool keepLooping = true;
        while (keepLooping) {
            char ch;
            fin.get(ch);                            // Get current byte's data

            if ((int)fin.tellg() <= 1) {             // If the data was at or before the 0th byte
                fin.seekg(0);                       // The first line is the last line
                keepLooping = false;                // So stop there
            }
            else if (ch == '\n') {                   // If the data was a newline
                keepLooping = false;                // Stop at the current position.
            }
            else {                                  // If the data was neither a newline nor at the 0 byte
                fin.seekg(-2, ios_base::cur);        // Move to the front of that data, then to the front of the data before it
            }
        }

        string lastLine;
        string lastLinePom;
        getline(fin, lastLine);

        while (getline(fin, lastLinePom)) {
            lastLine = lastLinePom;
        }
        
        _nameOfLast = lastLine.substr(0, lastLine.find("\t"));

        int start=-1; 
        int end=-1;

        for (int i = 0; i < _nameOfLast.length(); i++) {
            if (isdigit(_nameOfLast[i])) {
                if (start == -1) {
                    start = i;
                }
            }
            else if (start != -1) {
                end = i - 1;
                break;
            }
        }
        if (end == -1) {
            end = _nameOfLast.length() - 1;
        }

        _nameOfLast = _nameOfLast.substr(start, end);
        fin.close();
    }


    return stoi(_nameOfLast);
}
