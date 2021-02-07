#pragma once

#include <set>
#include "Laby.hh"
#include "Heldkarp.hh"

class Dijkstra
{
private:
    const Laby &laby;

public:
    std::map<Node, std::map<Node, Cost>> dist;
    std::map<Node, std::map<Node, std::optional<Node>>> prev;
    unsigned int counter = 0;
    unsigned int percent = 0;

    size_t indexOf(std::vector<Node> v, const Node & n);
    std::optional<Node> findClosestNode(const std::map<Node, Cost> & dist, const std::set<Node>& nodes);
    void printShortestPath(Heldkarp &heldkarp, Node start, Node target);
    void findShortestPath(Node start);

    Dijkstra(const Laby &laby);
};
