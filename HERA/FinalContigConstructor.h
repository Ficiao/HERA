#include "Buckets.h"
#include "Connection.h"

class FinalContigConstructor {
    Path finalPath;
public:
    void construct(std::vector<Bucket> buckets, std::string readsFilename, std::string contigsFilename,
                   std::string resultFilename);

    void writeToFile(std::string filename);

private:
    void addToFullPath(Bucket bucket);

    void getReadings(std::string filename);

    Connection findConnectionWithNode(int i, std::vector<Connection> connections) const;

    GraphNode *findNodeWithIndex(int i, bool contig) const;
};
