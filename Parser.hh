#ifndef PARSER_HH_INCLUDED
#define PARSER_HH_INCLUDED

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>

class Parser
{
public:
    std::string filename;

    int indexOf(std::vector<std::string> v, std::string s);
    int getLevelCost(std::string type);
    int getDeathCost(std::string type);
    std::string getComment(std::string type, std::string comment);
    void getLinks();
    void checkLink(int x1, int y1, std::string zone1, int x2, int y2, std::string zone2, std::string type);

    Parser(std::string filename);
};

#endif // PARSER_HH_INCLUDED
