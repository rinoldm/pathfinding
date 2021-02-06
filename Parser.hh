#ifndef PARSER_HH_INCLUDED
#define PARSER_HH_INCLUDED

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <tuple>
#include "Laby.hh"

class Laby;

class Parser
{
public:
    std::string linksfile;
    std::string mustpassfile;
    std::map<std::string, std::tuple<int, int, std::string>> linkData;

    int getLevelCost(std::string type);
    int getDeathCost(std::string type);
    uint8_t getCondition(std::string cond);
    std::string getComment(std::string type, std::string comment);
    void getMustPass(Laby &laby);
    void getLinks(Laby &laby);
    void checkLink(int x1, int y1, std::string zone1, int x2, int y2, std::string zone2, std::string type);

    Parser(std::string linksfile, std::string mustpassfile);
};

#endif // PARSER_HH_INCLUDED
