#include <queue>
#include "Dijkstra.hh"

Dijkstra::Dijkstra(const Laby &laby) : laby(laby) {}

size_t Dijkstra::indexOf(std::vector<Node> v, const StatefulNode &n) {
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] == n.toNode()) {
            return i * 8 + n.getState();
        }
    }
    std::string message("NotFound");
    throw message;
}

std::optional<StatefulNode>
Dijkstra::findClosestNode(const std::map<StatefulNode, Cost> &distance, const std::set<StatefulNode> &nodes) {
    Cost minDistanceFound = Cost::MAX;
    std::optional<StatefulNode> minNodeFound;

    for (const auto n : nodes) {
        if (distance.at(n) < minDistanceFound) {
            minDistanceFound = distance.at(n);
            minNodeFound = n;
        }
    }

    return minNodeFound;
}

//void Dijkstra::printShortestPath(Heldkarp &heldkarp, StatefulNode start, StatefulNode target) {
//    Cost cost = heldkarp.dist[this->indexOf(heldkarp.mustPass, start)][this->indexOf(heldkarp.mustPass, target)];
//
//    std::cout
//            << "Distance "
//            << this->laby.formatNode(start)
//            << " - "
//            << this->laby.formatNode(target)
//            << " : "
//            << cost.getDistance()
//            << std::endl;
//
//    std::vector<std::string> path;
//    for (;;) {
//        std::optional<StatefulNode> prev = this->prev[start][target];
//        if (!prev.has_value()) {
//            break;
//        }
//        Link link = laby.findLink(*prev, target);
//        path.insert(path.begin(), " " + laby.getLinkComment(link) + " -> " + laby.formatNode(target));
//        target = *prev;
//    }
//
//    for (auto i = path.begin(); i != path.end(); ++i) {
//        std::cout << *i << std::endl;
//    }
//    std::cout << std::endl;
//}

std::map<StatefulNode, Cost> Dijkstra::findShortestPath(StatefulNode start) {
    std::map<StatefulNode, Cost> dist;
    std::map<StatefulNode, std::optional<StatefulNode>> prev;

    typedef std::pair<Cost, StatefulNode> P;
    std::priority_queue<P, std::vector<P>, std::greater<>> unvisitedNodes;

    // Initialize all distances to +inf, and parents to "None"
    for (const auto &[node, _] : laby.getStatefulGraph()) {
        dist[node] = Cost::MAX;
        prev[node] = std::nullopt;
        if (node == start) {
            unvisitedNodes.push(std::make_pair(Cost(), node));
        } else {
            unvisitedNodes.push(std::make_pair(Cost::MAX, node));
        }
    }

    dist[start] = Cost();

    while (unvisitedNodes.size() > 0) {
        P top = unvisitedNodes.top();
        unvisitedNodes.pop();

        Cost cost = top.first;
        StatefulNode from = top.second;

        if (cost > dist[from]) {
            continue;
        }

        for (const auto &link : laby.getStatefulGraph().at(from)) {
            StatefulNode to = link.getTo();
            Cost oldCost = dist[to];
            Cost altCost = cost + link.getWeight();
            if (altCost < oldCost) {
                dist[to] = altCost;
                prev[to] = from;
                unvisitedNodes.push(std::make_pair(altCost, to));
            }
        }
    }

    this->dist[start] = dist;
    this->prev[start] = prev;

    return dist;
}

std::map<StatefulNode, std::map<StatefulNode, Cost>>
Dijkstra::shortestAllPairs(const std::vector<StatefulNode> &nodes) &{
    std::map<StatefulNode, std::map<StatefulNode, Cost>> dist;

    size_t processed = 0;
    for (const StatefulNode &from : nodes) {
        this->findShortestPath(from);
        for (const StatefulNode &to : nodes) {
            if ((from.getState() | to.getState()) == to.getState()) {
                dist[from][to] = this->dist[from][to];
            } else {
                dist[from][to] = Cost::MAX;
            }
        }
        processed++;
    }
    return dist;
}

std::deque<StatefulNode> Dijkstra::getPath(const StatefulNode &start, const StatefulNode &end) {
    std::deque<StatefulNode> path;

    std::optional<StatefulNode> cur = end;
    for(;;) {
        if (!cur.has_value()) {
            break;
        }
        path.push_front(*cur);
        cur = this->prev[start][*cur];
    }

    return path;
}
