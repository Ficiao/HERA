#include "GraphNode.h"
#include "Connection.h"

class Range {
public:
    double start;

    double end;

    Connection connection;

    bool contains(double number);

    Range(double start, double anEnd, Connection con);
};
