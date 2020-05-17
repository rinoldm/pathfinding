#ifndef HELDKARP_HH_INCLUDED
#define HELDKARP_HH_INCLUDED

#include <vector>
#include <array>
#include <iostream>

#define MUSTPASSNB 20
#define MAX_DISTANCE 666666

class Heldkarp
{
public:
    std::vector<std::string> mustPass;
    std::array<std::array<int, MUSTPASSNB>, MUSTPASSNB> dist;
    std::array<std::array<int, MUSTPASSNB>, 1 << MUSTPASSNB> visited;
    std::array<std::array<int, MUSTPASSNB>, 1 << MUSTPASSNB> previous;
    unsigned int counter = 0;
    unsigned int percent = 0;

    int findShortestTour(int end, int nodeBits);
    void printMatrix();

    Heldkarp();
};

#endif // HELDKARP_HH_INCLUDED
