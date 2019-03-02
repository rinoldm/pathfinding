#include "Heldkarp.hh"

#include "Laby.hh"
extern Laby laby;

int Heldkarp::findShortestTour(std::array<std::array<int, MUSTPASSNB + 1>, MUSTPASSNB + 1> dist, int end, std::set<int> nodes)
{
    int bits = 0;
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
        bits |= (1 << *it);

    if (this->visited[bits][end])
        return (this->visited[bits][end]);

    if (nodes.empty())
        return (this->visited[bits][end] = dist[end][0]);

    if (++this->counter > ((this->percent + 1) * ((1 << (MUSTPASSNB - 1)) * MUSTPASSNB - 2)) / 1000)
        ++this->percent && std::cout << "\r" << this->percent / 10 << "." << this->percent % 10 << "%";

    int minFound = MAX_DISTANCE;
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        std::set<int> newNodes(nodes);
        int newEnd = *it;
        newNodes.erase(newEnd);
        int newDist = dist[end][newEnd] + this->findShortestTour(dist, newEnd, newNodes);

        if (newDist < minFound)
        {
            for (auto it2 = laby.dependencies.begin(); it2 != laby.dependencies.end(); ++it2)
                if (newEnd == (*it2).first && !nodes.count((*it2).second))
                    newDist = MAX_DISTANCE;

            minFound = newDist;
            this->previous[bits][end] = newEnd;
        }
    }

    return (this->visited[bits][end] = minFound);
}
