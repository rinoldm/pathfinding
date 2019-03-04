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

void Laby::addLink(std::string from, std::string to, int weight, std::string comment, int death)
{
    if (this->graph.count(to) == 0)
        this->graph[to] = std::vector<Link>();

    this->graph[from].push_back(Link(from, to, weight, comment, death));
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
        "(-17;18)_bouton", "(-16;18)_bouton", "(-15;18)_bouton", "(-14;18)_bouton",
        "(-17;19)_bouton", "(-16;19)_bouton",                    "(-14;19)_bouton",
        "(-17;20)_bouton", "(-16;20)_bouton", "(-15;20)_bouton", "(-14;20)_bouton"
    };

    this->mustPass.insert(this->mustPass.begin(), this->entrance = "(0;0)");
    this->mustPass.insert(this->mustPass.end(),   this->exit     = "(-17;22)");

    if (this->mustPass.size() != MUSTPASSNB)
    {
        std::cout << "MUSTPASSNB INCORRECT" << std::endl;
        std::exit(1);
    }

    // TODO: dependencies
    // (-4;5)_boutons
    // (-5;10)_haut
    // (-9;5)_bouton

    parser.getLinks();
}
