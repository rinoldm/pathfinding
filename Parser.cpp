#include "Parser.hh"

#include "Laby.hh"
extern Laby laby;

Parser::Parser(std::string filename)
{
    this->filename = filename;
}

int Parser::indexOf(std::vector<std::string> v, std::string s)
{
    auto d = std::find(v.begin(), v.end(), s);
    if (d == v.end())
        std::cout << "INDEXOF ERROR " << s << std::endl;
    return (d - v.begin());
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
    std::map<std::string, int> levelCosts =
    {
        {"g", 1},
        {"d", 1},
        {"b", 1},
        {"c", 0},
        {"m", 0},
        {"M", 1},
        {"v", 1},
    };
    return (levelCosts[type]);
}

int Parser::getDeathCost(std::string type)
{
    std::map<std::string, int> deathCosts =
    {
        {"g", 0},
        {"d", 0},
        {"b", 0},
        {"c", 0},
        {"m", 1},
        {"M", 1},
        {"v", 0},
    };
    return (deathCosts[type]);
}

std::string Parser::getComment(std::string type, std::string comment)
{
    std::map<std::string, std::string> defaultComments =
    {
        {"g", "gauche"},
        {"d", "droite"},
        {"b", "bas"},
        {"c", "change zone"},
        {"m", "MORT"},
        {"M", "MORT"},
        {"v", "VORTEX"},
    };
   return (comment == "" ? defaultComments[type] : comment);
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
            std::cout << "LIEN INVALIDE : " << x1 << " " << y1 << " " << zone1 << " " << x2 << " " << y2 << " " << zone2 << " " << type << std::endl;
            std::exit(1);
        }
}

void Parser::getLinks()
{
    std::ifstream filestream(this->filename);
    std::string line;
    while (std::getline(filestream, line))
    {
        if ((trim(line)).size() == 0)
            continue;

        int x1, x2, y1, y2;
        std::string zone1, zone2, type, comment;

        std::istringstream linestream(line);
        std::string token;
        std::cout << "|" << line << "|" << std::endl;
        std::getline(linestream, token, '|');
        x1 = std::stoi(token);
        std::getline(linestream, token, '|');
        y1 = std::stoi(token);
        std::getline(linestream, token, '|');
        zone1 = trim(token);
        std::getline(linestream, token, '|');
        x2 = std::stoi(token);
        std::getline(linestream, token, '|');
        y2 = std::stoi(token);
        std::getline(linestream, token, '|');
        zone2 = trim(token);
        std::getline(linestream, token, '|');
        type = trim(token);
        std::getline(linestream, token, '|');
        comment = trim(token);

        this->checkLink(x1, y1, zone1, x2, y2, zone2, type);

        std::string from = std::string("(") + std::to_string(x1) + std::string(";") + std::to_string(y1) + std::string(")") + (zone1 != "" ? "_" + zone1 : "");
        std::string to   = std::string("(") + std::to_string(x2) + std::string(";") + std::to_string(y2) + std::string(")") + (zone2 != "" ? "_" + zone2 : "");
        laby.addLink(from, to, this->getLevelCost(type), false, this->getComment(type, comment), this->getDeathCost(type));
    }
}
