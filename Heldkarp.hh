#pragma once

#include <vector>
#include <array>
#include <iostream>
#include "Laby.hh"

#define MUSTPASSNB 17

class Heldkarp
{
private:
    const Laby& laby;
public:
    std::vector<Node> mustPass;
    std::array<std::array<Cost, MUSTPASSNB * 8>, MUSTPASSNB * 8> dist;
    std::array<std::array<Cost, MUSTPASSNB>, 1 << MUSTPASSNB> visited;
    std::array<std::array<size_t, MUSTPASSNB>, 1 << MUSTPASSNB> previous;
    unsigned int counter = 0;
    unsigned int percent = 0;

    Cost findShortestTour(int end, int nodeBits);
    void printMatrix();

    Heldkarp(const Laby &laby);
};
