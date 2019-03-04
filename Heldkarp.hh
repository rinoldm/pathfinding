#ifndef HELDKARP_HH_INCLUDED
#define HELDKARP_HH_INCLUDED

#include <set>

#include "Laby.hh"

class Heldkarp
{
public:
    std::array<std::array<int, MUSTPASSNB>, 1 << (MUSTPASSNB)> visited;
    std::array<std::array<int, MUSTPASSNB>, 1 << (MUSTPASSNB)> previous;
    int counter = 0;
    int percent = 0;

    int findShortestTour(std::array<std::array<int, MUSTPASSNB>, MUSTPASSNB> dist, int end, std::set<int> nodes);
};

#endif // HELDKARP_HH_INCLUDED
