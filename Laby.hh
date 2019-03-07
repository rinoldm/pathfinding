#ifndef LABY_HH_INCLUDED
#define LABY_HH_INCLUDED

#include <map>
#include <vector>
#include <algorithm>
#include "Parser.hh"

class Parser;

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
    std::map<std::string, std::vector<Link>> graph;
    std::string entrance;
    std::string exit;
    std::vector<std::string> mustPass;
    std::array<std::array<int, MUSTPASSNB>, MUSTPASSNB> dist;

    void addLink(std::string from, std::string to, int weight = 1, std::string comment = "", int death = 0);
    Link findLink(std::string from, std::string to);
    void printGraph();

    explicit Laby(Parser &parser);
};

#endif // LABY_HH_INCLUDED
