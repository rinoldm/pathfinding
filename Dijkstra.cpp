#include "Dijkstra.hh"

#include "Laby.hh"
extern Laby laby;

std::string Dijkstra::findClosestNode(std::map<std::string, int> dist, std::set<std::string> nodes)
{
    int minDistanceFound = MAX_DISTANCE;
    std::string minNodeFound = "UNDEFINED";

    for (auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        if (*i != "dummy" && dist[*i] < minDistanceFound)
        {
            minDistanceFound = dist[*i];
            minNodeFound = *i;
        }
    }

    return (minNodeFound);
}

void Dijkstra::printShortestPath(std::string start, std::string target)
{
    std::vector<std::string> path;
    while (target != start)
    {
        path.insert(path.begin(), " " + laby.findLink(prev[target], target).comment + " -> " + target);
        target = this->prev[target];
    }
    for (auto i = path.begin(); i != path.end(); ++i)
        std::cout << *i << std::endl;
    std::cout << std::endl;
}

int Dijkstra::findShortestPath(std::string start, std::string target, bool printPath)
{
    std::set<std::string> unvisitedNodes;
    std::map<std::string, int> dist;

    for (auto i = laby.graph.begin(); i != laby.graph.end(); ++i)
    {
        dist[(*i).first] = MAX_DISTANCE;
        prev[(*i).first] = "UNDEFINED";
        unvisitedNodes.insert((*i).first);
    }
    dist[start] = 0;

    while (unvisitedNodes.size() > 0)
    {
        std::string candidateNode = this->findClosestNode(dist, unvisitedNodes);

        if (candidateNode == "UNDEFINED")
            return (MAX_DISTANCE);

        unvisitedNodes.erase(candidateNode);

        if (candidateNode == target)
        {
            if (printPath)
                this->printShortestPath(start, target);
            return (dist[target]);
        }

        for (auto i = laby.graph.begin(); i != laby.graph.end(); ++i)
        {
            for (auto j = (*i).second.begin(); j != (*i).second.end(); ++j)
            {
                if (candidateNode == (*j).from)
                {
                    int alt = dist[candidateNode] + (*j).weight;
                    if (alt < dist[(*j).to])
                    {
                        dist[(*j).to] = alt;
                        prev[(*j).to] = candidateNode;
                    }
                }
            }
        }
    }
    return (MAX_DISTANCE);
}
