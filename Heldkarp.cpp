#include "Heldkarp.hh"

Heldkarp::Heldkarp(const Laby &laby) : laby(laby), mustPass(laby.getMustPass())
{
    if (this->mustPass.size() != MUSTPASSNB)
    {
        std::cout << "MUSTPASSNB INCORRECT" << std::endl;
        std::exit(1);
    }

    // TODO: dependencies
    // (-4;5)_boutons
    // (-5;10)_haut
    // (-9;5)_bouton
}

Cost Heldkarp::findShortestTour(int end, int nodeBits)
{
    if (this->visited[nodeBits][end] != Cost()) {
        return (this->visited[nodeBits][end]);
    }

    if (!nodeBits) {
        return (this->visited[nodeBits][end] = Cost());
    }

    if (++this->counter > ((this->percent + 1) * ((1 << (this->mustPass.size() - 1)) * this->mustPass.size() - this->mustPass.size())) / 100)
        ++this->percent && std::cout << "\r" << "Held-Karp : " << this->percent << "%";

    Cost minFound = Cost::MAX;
    for (unsigned int i = 0; i < this->mustPass.size(); ++i)
    {
        if (nodeBits & (1 << i))
        {
            Cost newDist = this->dist[end][i] + this->findShortestTour(i, nodeBits & ~(1 << i));
            if (newDist < minFound)
            {
                minFound = newDist;
                this->previous[nodeBits][end] = i;
            }
        }
    }

    return (this->visited[nodeBits][end] = minFound);
}

void Heldkarp::printMatrix()
{
    std::cout << "Distance matrix:" << std::endl;
    for (auto i = this->dist.begin(); i != this->dist.end(); ++i)
    {
        std::cout << this->laby.formatNode(this->mustPass[i - this->dist.begin()]) << " ";
        for (auto & j : *i) {
            std::cout << j.getDistance() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
