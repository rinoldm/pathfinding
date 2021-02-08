#include "Parser.hh"

Parser::Parser(std::string linksfile, std::string nodesfile) : linksfile(linksfile), nodesfile(nodesfile) {
    this->linkData =
    {
        {"g", std::make_tuple(1, 0, "gauche")},
        {"d", std::make_tuple(1, 0, "droite")},
        {"b", std::make_tuple(1, 0, "bas")},
        {"c", std::make_tuple(0, 0, "change zone")},
        {"m", std::make_tuple(0, 1, "MORT")},
        {"M", std::make_tuple(1, 1, "MORT")},
        {"v", std::make_tuple(1, 0, "VORTEX")},
    };
}

static inline std::string trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

uint16_t Parser::getLevelCost(std::string type)
{
    return (std::get<0>(this->linkData[type]));
}

uint16_t Parser::getDeathCost(std::string type)
{
    return (std::get<1>(this->linkData[type]));
}

uint8_t Parser::getCondition(std::string cond)
{
    uint8_t condition = 0b111111;

    if (cond == "A") {
        condition &= ~(1 << 0);
    }
    else if (cond == "B") {
        condition &= ~(1 << 2);
    }
    else if (cond == "C") {
        condition &= ~(1 << 4);
    }
    else if (cond != "") {
        std::string message("Condition inconnue");
        throw message;
    }
    return condition;
}

std::string Parser::getComment(std::string type, std::string comment)
{
   return (comment == "" ? std::get<2>(this->linkData[type]) : comment);
}

void Parser::checkLink(int x1, int y1, std::string zone1, int x2, int y2, std::string zone2, std::string type)
{
    if ((type == "g" && (x2 != x1 - 1 || y2 != y1    )) ||
        (type == "d" && (x2 != x1 + 1 || y2 != y1    )) ||
        (type == "b" && (x2 != x1     || y2 != y1 + 1)) ||
        (type == "c" && (x2 != x1     || y2 != y1       || zone2 == zone1)) ||
        (type == "m" && (x2 != x1     || y2 != y1       || zone2 == zone1)) ||
        (type == "M" && (x2 == x1     && y2 == y1    )) ||
        (type == "v" && (x2 == x1     && y2 == y1    )))
        {
            std::cerr << "LIEN INVALIDE : " << x1 << " " << y1 << " " << zone1 << " " << x2 << " " << y2 << " " << zone2 << " " << type << std::endl;
            std::exit(1);
        }
}

void Parser::getMustPass(Laby &laby)
{
    std::ifstream filestream(this->nodesfile);
    if(filestream.fail()){
        std::string message("I/O Error getMustPass");
        std::cerr << message << std::endl;
        throw message;
    }
    std::string line;
    while (std::getline(filestream, line))
    {
        if ((trim(line)).empty()) {
            continue;
        }

        int x, y;
        std::string zone, flag;
        std::istringstream linestream(line);
        std::string token;

        std::getline(linestream, token, '|');   x = std::stoi(token);
        std::getline(linestream, token, '|');   y = std::stoi(token);
        std::getline(linestream, token, '|');   zone = trim(token);
        std::getline(linestream, token, '|');   flag = trim(token);

        Node node = laby.makeNode(x, y, zone);

        if (flag == "m")
        {   
            laby.addMustPass(node);
        }
        else
        {
            laby.addTransition(node);
        }
        
    }
}

void Parser::getLinks(Laby &laby)
{
    std::ifstream filestream(this->linksfile);
    if(filestream.fail()){
        std::string message("I/O Error getLinks");
        std::cerr << message << std::endl;
        throw message;
    }
    std::string line;
    while (std::getline(filestream, line))
    {
        if ((trim(line)).empty()) {
            continue;
        }

        int x1, x2, y1, y2;
        std::string zone1, cond, zone2, type, comment;
        std::istringstream linestream(line);
        std::string token;

        std::getline(linestream, token, '|');   x1 = std::stoi(token);
        std::getline(linestream, token, '|');   y1 = std::stoi(token);
        std::getline(linestream, token, '|');   zone1 = trim(token);
        std::getline(linestream, token, '|');   cond = trim(token);
        std::getline(linestream, token, '|');   x2 = std::stoi(token);
        std::getline(linestream, token, '|');   y2 = std::stoi(token);
        std::getline(linestream, token, '|');   zone2 = trim(token);
        std::getline(linestream, token, '|');   type = trim(token);
        std::getline(linestream, token, '|');   comment = trim(token);

        this->checkLink(x1, y1, zone1, x2, y2, zone2, type);

        Node from = laby.makeNode(x1, y1, zone1);
        Node to = laby.makeNode(x2, y2, zone2);
        Cost weight = Cost(this->getLevelCost(type), this->getDeathCost(type));

        laby.addLink(from, to, weight, this->getCondition(cond), this->getComment(type, comment));
    }
}

Laby Parser::parse() {
    Laby laby;
    this->getLinks(laby);
    this->getMustPass(laby);
    laby.finalizeGraph();
    return laby;
}
