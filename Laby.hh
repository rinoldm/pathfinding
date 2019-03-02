#ifndef LABY_HH_INCLUDED
#define LABY_HH_INCLUDED

#include <iostream>
#include <map>
#include <vector>

#define MUSTPASSNB 17
#define MAX_DISTANCE 666666

class Link
{
public:
    std::string from;
    std::string to;
    int weight;
    std::string comment;
    int death;

    Link(std::string from, std::string to, int weight, std::string comment, int death);
};

class Laby
{
public:
    std::map<std::string, std::vector<Link> > graph;
    std::string entrance;
    std::string exit;
    std::vector<std::string> mustPass;
    std::vector<std::pair<int, int>> dependencies;
    std::array<std::array<int, MUSTPASSNB + 1>, MUSTPASSNB + 1> dist;

    void addLink(std::string from, std::string to, int weight, bool bothways = false, std::string comment = "", int death = 0);
    void addDummy();
    Link findLink(std::string from, std::string to);
    void printGraph();

    Laby();
};

#endif // LABY_HH_INCLUDED
