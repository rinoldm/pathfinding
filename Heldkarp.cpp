#include "Heldkarp.hh"

int Heldkarp::findShortestTour(std::array<std::array<int, MUSTPASSNB>, MUSTPASSNB> dist, int end, int nodeBits)
{
    if (this->visited[nodeBits][end])
        return (this->visited[nodeBits][end]);

    if (!nodeBits)
        return (this->visited[nodeBits][end] = 0);

    if (++this->counter > ((this->percent + 1) * ((1 << (MUSTPASSNB - 1)) * MUSTPASSNB - MUSTPASSNB)) / 100)
        ++this->percent && std::cout << "\r" << "Held-Karp : " << this->percent << "%";

    int minFound = MAX_DISTANCE;
    for (int i = 0; i < MUSTPASSNB; ++i)
    {
        if (nodeBits & (1 << i))
        {
            int newDist = dist[end][i] + this->findShortestTour(dist, i, nodeBits & ~(1 << i));
            if (newDist < minFound)
            {
                minFound = newDist;
                this->previous[nodeBits][end] = i;
            }
        }
    }

    return (this->visited[nodeBits][end] = minFound);
}
