#include "Laby.hh"
#include "Dijkstra.hh"
#include "Heldkarp.hh"
#include "Parser.hh"

Heldkarp heldkarp;

int main()
{
    Parser parser("links.txt");
    Laby laby(parser);
    Dijkstra dijkstra(laby);

    for (unsigned int i = 0; i != MUSTPASSNB; ++i)
    {
        dijkstra.findShortestPath(laby.mustPass[i]);
        for (unsigned int j = 0; j != MUSTPASSNB; ++j)
            laby.dist[i][j] = dijkstra.dist[laby.mustPass[i]][laby.mustPass[j]];
    }
    std::cout << std::endl;

    // laby.printGraph();

    int answer = heldkarp.findShortestTour(laby.dist, 0, (1 << MUSTPASSNB) - 1) + 1;
    std::cout << std::endl << "Nombre de niveaux parcourus : " << answer << std::endl << std::endl;

    std::vector<std::string> path = {laby.entrance};
    for (int nodeBits = (1 << MUSTPASSNB) - 1, end = 0; nodeBits != 0; nodeBits &= ~(1 << end))
        path.push_back(laby.mustPass[end = heldkarp.previous[nodeBits][end]]);

    std::cout << "Entree du labyrinthe : " << laby.entrance << std::endl << std::endl;
    for (unsigned int i = 1; i < path.size() - 1; ++i)
        dijkstra.printShortestPath(path[i], path[i + 1]);
    std::cout << "Sortie du labyrinthe : " << laby.exit << std::endl;

    return 0;
}
