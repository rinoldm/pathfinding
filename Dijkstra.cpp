#include <iostream>
#include "Dijkstra.hh"
#include "Laby.hh"

int Dijkstra::indexOf(std::vector<std::string> v, std::string s)
{
    auto d = std::find(v.begin(), v.end(), s);
    if (d == v.end())
        std::cout << "INDEXOF ERROR " << s << std::endl;
    return (d - v.begin());
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
    std::cout << "Distance " << start << " - " << target << " : " << laby.dist[this->indexOf(laby.mustPass, start)][this->indexOf(laby.mustPass, target)] << std::endl;

    std::vector<std::string> path;
    while (target != start)
    {
        path.insert(path.begin(), " " + laby.findLink(prev[start][target], target).comment + " -> " + target);
        target = this->prev[start][target];
    }
    for (auto i = path.begin(); i != path.end(); ++i)
        std::cout << *i << std::endl;
    std::cout << std::endl;
}

void Dijkstra::findShortestPath(std::string start)
{
    std::set<std::string> unvisitedNodes;

    for (auto i = laby.graph.begin(); i != laby.graph.end(); ++i)
    {
        dist[start][(*i).first] = MAX_DISTANCE;
        prev[start][(*i).first] = "UNDEFINED";
        unvisitedNodes.insert((*i).first);
    }
    dist[start][start] = 0;

    if (++this->counter > ((this->percent * MUSTPASSNB) / 100))
        ++this->percent && std::cout << "\r" << "Dijkstra : " << this->percent << "/" << MUSTPASSNB;

    while (unvisitedNodes.size() > 0)
    {
        std::string candidateNode = this->findClosestNode(dist[start], unvisitedNodes);
        if (candidateNode == "UNDEFINED")
            break;
        unvisitedNodes.erase(candidateNode);

        for (auto i = laby.graph.begin(); i != laby.graph.end(); ++i)
        {
            for (auto j = (*i).second.begin(); j != (*i).second.end(); ++j)
            {
                if (candidateNode == (*j).from)
                {
                    int alt = dist[start][candidateNode] + (*j).weight;
                    if (alt < dist[start][(*j).to])
                    {
                        dist[start][(*j).to] = alt;
                        prev[start][(*j).to] = candidateNode;
                    }
                }
            }
        }
    }
}

Dijkstra::Dijkstra(Laby &laby): laby(laby) {}
