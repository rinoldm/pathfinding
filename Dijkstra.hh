#ifndef DIJKSTRA_HH_INCLUDED
#define DIJKSTRA_HH_INCLUDED

#include <map>
#include <set>

class Dijkstra
{
public:
    std::map<std::string, std::string> prev;

    std::string findClosestNode(std::map<std::string, int> dist, std::set<std::string> nodes);
    void printShortestPath(std::string start, std::string target);
    int findShortestPath(std::string start, std::string target, bool printPath = false);
};

#endif // DIJKSTRA_HH_INCLUDED
