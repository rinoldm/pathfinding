#ifndef LABY_HH_INCLUDED
#define LABY_HH_INCLUDED

#include <map>
#include <vector>
#include "Parser.hh"

class Parser;

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

    void addLink(std::string from, std::string to, int weight = 1, std::string comment = "", int death = 0);
    Link findLink(std::string from, std::string to);
    void printGraph();

    Laby(Parser &parser);
};

#endif // LABY_HH_INCLUDED
