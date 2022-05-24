#include "GraphNode.h"

class ConnectionsSorter {
public:
    GraphNode* sortConnectionsByOverlapScore(GraphNode* _nodes);
    GraphNode* sortConnectionsByExtensionScore(GraphNode* _nodes);
};
