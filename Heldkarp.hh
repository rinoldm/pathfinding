#pragma once

#include <vector>
#include <array>
#include <iostream>
#include "Laby.hh"

#define MUSTPASSNB 17

class Heldkarp
{
private:
    const Laby& laby;
    size_t NODE_COUNT;
    size_t STATEFUL_NODE_COUNT;

    std::map<Node, size_t> nodeToId;
    std::map<StatefulNode, size_t> statefulNodeToId;

    // Cost for each pair of `StatefulNode`
    std::vector<Cost> costs;
    // Cost of the current tour, or `Cost()` is unknown.
    std::vector<Cost> tourCosts;
    // Previous st node for a given held-karp iteration
    std::vector<size_t> previous;

    Cost getCost(const StatefulNode & from, const StatefulNode & to);
    Cost getCost(size_t from, size_t to);

    Cost getTourCost(uint32_t visitedSet, size_t lastStNodeId);
    void setTourCost(uint32_t visitedSet, size_t lastStNodeId, Cost c);

    void setPrevious(uint32_t visitedSet, size_t lastStNodeId, size_t prevId);

    Cost innerFindShortestTour(uint32_t visitedSet, size_t lastStNodeId);

public:
    unsigned int counter = 0;
    unsigned int percent = 0;

    Cost findShortestTour();

    Heldkarp(const Laby &laby, const std::map<StatefulNode, std::map<StatefulNode, Cost>> & dist);
};
