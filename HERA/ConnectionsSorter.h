// @Author Fribert

#include "GraphNode.h"

class ConnectionsSorter {
public:
    GraphNode *sortConnectionsByOverlapScore(GraphNode *_nodes, int size);
    GraphNode *sortConnectionsByExtensionScore(GraphNode *_nodes, int size);
};
