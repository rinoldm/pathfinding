#ifndef DIJKSTRA_HH_INCLUDED
#define DIJKSTRA_HH_INCLUDED

#include <set>
#include "Laby.hh"
#include "Heldkarp.hh"

class Dijkstra
{
private:
    Laby &laby;

public:
    std::map<std::string, std::map<std::string, int>> dist;
    std::map<std::string, std::map<std::string, std::string>> prev;
    unsigned int counter = 0;
    unsigned int percent = 0;

    int indexOf(std::vector<std::string> v, std::string s);
    std::string findClosestNode(std::map<std::string, int> dist, std::set<std::string> nodes);
    void printShortestPath(Heldkarp &heldkarp, std::string start, std::string target);
    void findShortestPath(std::string start);

    Dijkstra(Laby &laby);
};

#endif // DIJKSTRA_HH_INCLUDED
