#include "Dijkstra.hh"

#include "Heldkarp.hh"
extern Heldkarp heldkarp;

Dijkstra::Dijkstra(Laby &laby): laby(laby) {}

int Dijkstra::indexOf(std::vector<std::string> v, std::string s)
{
    return (std::find(v.begin(), v.end(), s) - v.begin());
}

std::string Dijkstra::findClosestNode(std::map<std::string, int> distance, std::set<std::string> nodes)
{
    int minDistanceFound = MAX_DISTANCE;
    std::string minNodeFound = "UNDEFINED";

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        if (distance[*i] < minDistanceFound)
        {
            minDistanceFound = distance[*i];
            minNodeFound = *i;
        }
    }

    return (minNodeFound);
}

void Dijkstra::printShortestPath(std::string start, std::string target)
{
    std::cout << "Distance " << start << " - " << target << " : " << heldkarp.dist[this->indexOf(heldkarp.mustPass, start)][this->indexOf(heldkarp.mustPass, target)] << std::endl;

    std::vector<std::string> path;
    for (; target != start; target = this->prev[start][target])
        path.insert(path.begin(), " " + laby.findLink(this->prev[start][target], target).comment + " -> " + target);

    for (auto i = path.begin(); i != path.end(); ++i)
        std::cout << *i << std::endl;
    std::cout << std::endl;
}

void Dijkstra::findShortestPath(std::string start)
{
    std::set<std::string> unvisitedNodes;

    for (auto i = laby.graph.begin(); i != laby.graph.end(); ++i)
    {
        this->dist[start][(*i).first] = MAX_DISTANCE;
        this->prev[start][(*i).first] = "UNDEFINED";
        unvisitedNodes.insert((*i).first);
    }
    this->dist[start][start] = 0;

    if (++this->counter > ((this->percent * heldkarp.mustPass.size()) / 100))
        ++this->percent && std::cout << "\r" << "Dijkstra : " << this->percent << "/" << heldkarp.mustPass.size();

    while (unvisitedNodes.size() > 0)
    {
        std::string candidateNode = this->findClosestNode(this->dist[start], unvisitedNodes);
        if (candidateNode == "UNDEFINED")
            break;
        unvisitedNodes.erase(candidateNode);

        for (auto i = laby.graph.begin(); i != laby.graph.end(); ++i)
        {
            for (auto j = (*i).second.begin(); j != (*i).second.end(); ++j)
            {
                if (candidateNode == (*j).from)
                {
                    int alt = this->dist[start][candidateNode] + (*j).weight;
                    if (alt < this->dist[start][(*j).to])
                    {
                        this->dist[start][(*j).to] = alt;
                        this->prev[start][(*j).to] = candidateNode;
                    }
                }
            }
        }
    }
}
