// @Author Fribert

#include "GraphNode.h"
#include "Connection.h"

class Range {
public:
    int start;

    int end;

    Connection connection;

    bool contains(int number);

    Range(int start, int anEnd, Connection con);
};