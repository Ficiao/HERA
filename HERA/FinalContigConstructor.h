#include <map>
#include "Buckets.h"
#include "Connection.h"

class FinalContigConstructor {
public:
    std::map<int, std::string> reads;
    std::map<int, std::string> contigs;

    void construct(std::vector<Path> paths, std::string readsFilename, std::string contigsFilename,
              std::string resultFilename);

private:
    void loadFile(std::string filename);

    Connection findConnectionWithNode(int i, std::vector<Connection> connections) const;
};
