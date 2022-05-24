#include <algorithm>
#include "ConnectionsSorter.h"
#include "Connection.h"

GraphNode *ConnectionsSorter::sortConnectionsByOverlapScore(GraphNode *_nodes) {
    for (int i = 0; i < _nodes->size; ++i) {
        std::sort(_nodes[i].connections.begin(),
                  _nodes[i].connections.end(),
                  [](const Connection &left, const Connection &right) {
                      return left.overlapScore > right.overlapScore;
                  });

        std::sort(_nodes[i].backwardsContigConnection.begin(),
                  _nodes[i].backwardsContigConnection.end(),
                  [](const Connection &left, const Connection &right) {
                      return left.overlapScore > right.overlapScore;
                  });
    }

    return _nodes;
}

GraphNode *ConnectionsSorter::sortConnectionsByExtensionScore(GraphNode *_nodes) {
    for (int i = 0; i < _nodes->size; ++i) {
        std::sort(_nodes[i].connections.begin(),
                  _nodes[i].connections.end(),
                  [](const Connection &left, const Connection &right) {
                      return left.extensionScore > right.extensionScore;
                  });

        std::sort(_nodes[i].backwardsContigConnection.begin(),
                  _nodes[i].backwardsContigConnection.end(),
                  [](const Connection &left, const Connection &right) {
                      return left.extensionScore > right.extensionScore;
                  });
    }

    return _nodes;
}
