#include "Parser.hh"

Parser::Parser(std::string linksfile, std::string mustpassfile) : linksfile(linksfile), mustpassfile(mustpassfile) {
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

int Parser::getLevelCost(std::string type)
{
    return (std::get<0>(this->linkData[type]));
}

int Parser::getDeathCost(std::string type)
{
    return (std::get<1>(this->linkData[type]));
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
    std::ifstream filestream(this->mustpassfile);
    if(filestream.fail()){
        std::string message("I/O Error getMustPass");
        std::cerr << message << std::endl;
        throw message;
    }
    std::string line;
    while (std::getline(filestream, line))
    {
        laby.addMustPass(trim(line));
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
        if ((trim(line)).size() == 0)
            continue;

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

        std::string from = std::string("(") + std::to_string(x1) + std::string(";") + std::to_string(y1) + std::string(")") + (zone1 != "" ? "_" + zone1 : "");
        std::string to   = std::string("(") + std::to_string(x2) + std::string(";") + std::to_string(y2) + std::string(")") + (zone2 != "" ? "_" + zone2 : "");
        laby.addLink(from, to, this->getLevelCost(type), 0b111111, this->getComment(type, comment), this->getDeathCost(type));
    }
}
