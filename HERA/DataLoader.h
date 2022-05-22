#pragma once
#include <string>

class DataLoader {
public:
	void LoadData(std::string _fileName);
private:
	int IndexOfLastElem(std::string& _fileName);
};