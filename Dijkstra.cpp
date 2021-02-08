#include "Dijkstra.hh"

Dijkstra::Dijkstra(const Laby &laby): laby(laby) {}

size_t Dijkstra::indexOf(std::vector<Node> v, const StatefulNode & n)
{
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] == n.toNode()) {
            return i * 8 + n.getState();
        }
    }
    std::string message("NotFound");
    throw message;
}

std::optional<StatefulNode> Dijkstra::findClosestNode(const std::map<StatefulNode, Cost> & distance, const std::set<StatefulNode>& nodes)
{
    Cost minDistanceFound = Cost::MAX;
    std::optional<StatefulNode> minNodeFound;

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

void Dijkstra::printShortestPath(Heldkarp &heldkarp, StatefulNode start, StatefulNode target)
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
        std::optional<StatefulNode> prev = this->prev[start][target];
        if (!prev.has_value()) {
            break;
        }
        Link link = laby.findLink(*prev, target);
        path.insert(path.begin(), " " + laby.getLinkComment(link) + " -> " + laby.formatNode(target));
        target = *prev;
    }

    for (auto i = path.begin(); i != path.end(); ++i) {
        std::cout << *i << std::endl;
    }
    std::cout << std::endl;
}

void Dijkstra::findShortestPath(StatefulNode start)
{
    std::set<StatefulNode> unvisitedNodes;

    for (const auto & [from, links] : laby.getStatefulGraph())
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
        std::optional<StatefulNode> candidateNode = this->findClosestNode(this->dist[start], unvisitedNodes);
        if (!candidateNode.has_value()) {
            break;
        }
        unvisitedNodes.erase(*candidateNode);

        for (const auto & [from, outLinks] : laby.getStatefulGraph())
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
