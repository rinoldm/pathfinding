#include "Dijkstra.hh"

Dijkstra::Dijkstra(const Laby &laby): laby(laby) {}

int Dijkstra::indexOf(std::vector<Node> v, Node s)
{
    return (std::find(v.begin(), v.end(), s) - v.begin());
}

std::optional<Node> Dijkstra::findClosestNode(const std::map<Node, Cost> & distance, const std::set<Node>& nodes)
{
    Cost minDistanceFound = Cost::MAX;
    std::optional<Node> minNodeFound;

    for (const auto n : nodes)
    {
        if (distance.at(n) < minDistanceFound)
        {
            minDistanceFound = distance.at(n);
            minNodeFound = n;
        }
    }

    return minNodeFound;
}

void Dijkstra::printShortestPath(Heldkarp &heldkarp, Node start, Node target)
{
    Cost cost = heldkarp.dist[this->indexOf(heldkarp.mustPass, start)][this->indexOf(heldkarp.mustPass, target)];

    std::cout
        << "Distance "
        << this->laby.formatNode(start)
        << " - "
        << this->laby.formatNode(target)
        << " : "
        << cost.getDistance()
        << std::endl;

    std::vector<std::string> path;
    for (;;) {
        std::optional<Node> prev = this->prev[start][target];
        if (!prev.has_value()) {
            break;
        }
        ConditionalLink link = laby.findLink(*prev, target);
        path.insert(path.begin(), " " + laby.getLinkComment(link) + " -> " + laby.formatNode(target));
        target = *prev;
    }

    for (auto i = path.begin(); i != path.end(); ++i) {
        std::cout << *i << std::endl;
    }
    std::cout << std::endl;
}

void Dijkstra::findShortestPath(Node start)
{
    std::set<Node> unvisitedNodes;

    for (const auto & [from, links] : laby.getGraph())
    {
        this->dist[start][from] = Cost::MAX;
        this->prev[start][from] = std::nullopt;
        unvisitedNodes.insert(from);
    }
    this->dist[start][start] = Cost();

    size_t mustPassCount = laby.getMustPass().size();

    if (++this->counter > ((this->percent * mustPassCount) / 100))
        ++this->percent && std::cout << "\r" << "Dijkstra : " << this->percent << "/" << mustPassCount;

    while (unvisitedNodes.size() > 0)
    {
        std::optional<Node> candidateNode = this->findClosestNode(this->dist[start], unvisitedNodes);
        if (!candidateNode.has_value()) {
            break;
        }
        unvisitedNodes.erase(*candidateNode);

        for (const auto & [from, outLinks] : laby.getGraph())
        {
            for (const auto & link : outLinks)
            {
                if (candidateNode == link.getFrom())
                {
                    Cost alt = this->dist[start][*candidateNode] + link.getWeight();
                    if (alt < this->dist[start][link.getTo()])
                    {
                        this->dist[start][link.getTo()] = alt;
                        this->prev[start][link.getTo()] = candidateNode;
                    }
                }
            }
        }
    }
}
