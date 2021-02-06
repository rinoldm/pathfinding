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
    std::vector<std::string> mustPass;

    void addLink(std::string from, std::string to, int weight = 1, std::string comment = "", int death = 0);
    Link findLink(std::string from, std::string to);
    void addMustPass(std::string mustpass);
    void printGraph();

    Laby(Parser &parser);
};

#endif // LABY_HH_INCLUDED
