#include "Heldkarp.hh"

Heldkarp::Heldkarp(Laby &laby)
{
    this->mustPass = laby.mustPass;

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

int Heldkarp::findShortestTour(int end, int nodeBits)
{
    if (this->visited[nodeBits][end])
        return (this->visited[nodeBits][end]);

    if (!nodeBits)
        return (this->visited[nodeBits][end] = 0);

    if (++this->counter > ((this->percent + 1) * ((1 << (this->mustPass.size() - 1)) * this->mustPass.size() - this->mustPass.size())) / 100)
        ++this->percent && std::cout << "\r" << "Held-Karp : " << this->percent << "%";

    int minFound = MAX_DISTANCE;
    for (unsigned int i = 0; i < this->mustPass.size(); ++i)
    {
        if (nodeBits & (1 << i))
        {
            int newDist = this->dist[end][i] + this->findShortestTour(i, nodeBits & ~(1 << i));
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
        std::cout << this->mustPass[i - this->dist.begin()] << " ";
        for (auto j = (*i).begin(); j != (*i).end(); ++j)
            std::cout << *j << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
