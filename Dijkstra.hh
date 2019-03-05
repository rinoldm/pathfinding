#ifndef DIJKSTRA_HH_INCLUDED
#define DIJKSTRA_HH_INCLUDED

#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include "Laby.hh"

class Dijkstra
{
private:
    Laby &laby;
public:
    std::map<std::string, std::string> prev;
    int counter = 0;
    int percent = 0;

    int indexOf(std::vector<std::string> v, std::string s);
    std::string findClosestNode(std::map<std::string, int> dist, std::set<std::string> nodes);
    void printShortestPath(std::string start, std::string target);
    int findShortestPath(std::string start, std::string target, bool printPath = false);
    explicit Dijkstra(Laby &laby);
};

#endif // DIJKSTRA_HH_INCLUDED
