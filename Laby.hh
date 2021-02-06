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

    // mask describing the necessary conditions to use this link
    // 0 : (-4;5)_boutons disabled
    // 1 : (-4;5)_boutons enabled
    // 2 : (-5;10)_haut disabled
    // 3 : (-5;10)_haut enabled
    // 4 : (-9;5)_bouton disabled
    // 5 : (-9;5)_bouton enabled
    uint8_t condition;

    std::string comment;
    int death;

    Link(std::string from, std::string to, int weight, uint8_t condition, std::string comment, int death);
};

class Laby
{
public:
    std::map<std::string, std::vector<Link>> graph;
    std::vector<std::string> mustPass;

    void addLink(std::string from, std::string to, int weight = 1, uint8_t condition = 0b111111, std::string comment = "", int death = 0);
    Link findLink(std::string from, std::string to);
    void addMustPass(std::string mustpass);
    void printGraph();

    Laby(Parser &parser);
};

#endif // LABY_HH_INCLUDED
