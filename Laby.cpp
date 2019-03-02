#include "Laby.hh"

#include "Parser.hh"
extern Parser parser;

Link::Link(std::string from, std::string to, int weight, std::string comment, int death)
{
    this->from = from;
    this->to = to;
    this->weight = weight;
    this->comment = comment;
    this->death = death;
}

void Laby::addLink(std::string from, std::string to, int weight, bool bothways, std::string comment, int death)
{
    if (this->graph.count(to) == 0)
        this->graph[to] = std::vector<Link>();

    this->graph[from].push_back(Link(from, to, weight, comment, death));
    if (bothways)
        this->graph[to].push_back(Link(to, from, weight, comment, death));
}

void Laby::addDummy()
{
    this->mustPass.push_back("dummy");

    for (int i = 0; i < MUSTPASSNB; ++i)
    {
        this->dist[MUSTPASSNB][i] = MAX_DISTANCE;
        this->dist[i][MUSTPASSNB] = MAX_DISTANCE;
    }

    this->addLink(this->mustPass[this->mustPass.size() - 2], "dummy", 0);
    this->dist[MUSTPASSNB - 1][MUSTPASSNB] = 0;

    this->addLink("dummy", this->mustPass[0], 0);
    this->dist[MUSTPASSNB][0] = 0;

}

Link Laby::findLink(std::string from, std::string to)
{
    for (auto i = this->graph[from].begin(); i != this->graph[from].end(); ++i)
        if ((*i).to == to)
            return (*i);
}

void Laby::printGraph()
{
    std::cout << "List of nodes and links:" << std::endl;
    for (auto i = this->graph.begin(); i != this->graph.end(); ++i)
    {
        std::cout << (*i).first << std::endl;
        for (auto j = (*i).second.begin(); j != (*i).second.end(); ++j)
            std::cout << "  -> " << (*j).to << " " << (*j).weight << " " << (*j).death << " " << (*j).comment << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Distance matrix:" << std::endl;
    for (auto i = this->dist.begin(); i != this->dist.end(); ++i)
    {
        std::cout << this->mustPass[i - this->dist.begin()] << " ";
        for (auto j = (*i).begin(); j != (*i).end(); ++j)
            std::cout << *j << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Laby::Laby()
{
    this->mustPass =
    {
        "(-17;17)_bouton", "(-16;17)_bouton", "(-15;17)_bouton", "(-14;17)_bouton",
        "(-14;18)_bouton", "(-15;18)_bouton", "(-16;18)_bouton", "(-17;18)_bouton",
        "(-17;19)_bouton", "(-16;19)_bouton",                    "(-14;19)_bouton",
        "(-14;20)_bouton", "(-15;20)_bouton", "(-16;20)_bouton", "(-17;20)_bouton"
    };

    this->mustPass.insert(this->mustPass.begin(), this->entrance =  "(0;0)");
    this->mustPass.insert(this->mustPass.end(),   this->exit     = "(-17;22)");

    if (this->mustPass.size() != MUSTPASSNB)
        std::cout << "MUSTPASSNB INCORRECT" << std::endl;

    std::vector<std::pair<std::string, std::string>> dep =
    {
        //std::make_pair("(0;2)", "(0;3)"),
        //std::make_pair("(-3;0)", "(-1;0)"),
        //std::make_pair("(-1;2)", "(0;2)"),
        //std::make_pair("(-2;2)_tp", "(-2;2)_main")
    };

    for (auto it = dep.begin(); it != dep.end(); ++it)
        this->dependencies.push_back(std::make_pair(parser.indexOf(this->mustPass, (*it).first), parser.indexOf(this->mustPass, (*it).second)));

    parser.getLinks();
}
