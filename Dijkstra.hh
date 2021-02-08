#pragma once

#include <set>
#include "Laby.hh"
#include "Heldkarp.hh"

class Dijkstra
{
private:
    const Laby &laby;

public:
    std::map<StatefulNode, std::map<StatefulNode, Cost>> dist;
    std::map<StatefulNode, std::map<StatefulNode, std::optional<StatefulNode>>> prev;
    unsigned int counter = 0;
    unsigned int percent = 0;

    size_t indexOf(std::vector<Node> v, const StatefulNode & n);
    std::optional<StatefulNode> findClosestNode(const std::map<StatefulNode, Cost> & dist, const std::set<StatefulNode>& nodes);
    void printShortestPath(Heldkarp &heldkarp, StatefulNode start, StatefulNode target);
    std::map<StatefulNode, Cost> findShortestPath(StatefulNode start);

    // map[from][to] = cost;
    std::map<StatefulNode, std::map<StatefulNode, Cost>> shortestAllPairs(const Laby & laby, const std::vector<StatefulNode> & nodes) &;

    Dijkstra(const Laby &laby);
};
