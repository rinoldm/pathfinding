#include "Laby.hh"

Link::Link(std::string from, std::string to, int weight, std::string comment, int death) : from(from), to(to), weight(weight), comment(comment), death(death) {}

Laby::Laby(Parser &parser)
{
    parser.getLinks(*this);
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

void Laby::addMustPass(std::string mustpass)
{
     this->mustPass.push_back(mustpass);
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
}
