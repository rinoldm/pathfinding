#pragma once

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
    std::map<std::string, std::tuple<uint16_t, uint16_t, std::string>> linkData;

    uint16_t getLevelCost(std::string type);
    uint16_t getDeathCost(std::string type);
    uint8_t getCondition(std::string cond);
    std::string getComment(std::string type, std::string comment);
    void checkLink(int x1, int y1, std::string zone1, int x2, int y2, std::string zone2, std::string type);

    Laby parse();

    Parser(std::string linksfile, std::string mustpassfile);
private:
    void getMustPass(Laby &laby);
    void getLinks(Laby &laby);
};
