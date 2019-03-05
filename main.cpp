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
        for (unsigned int j = 0; j != MUSTPASSNB; ++j)
            laby.dist[i][j] = dijkstra.findShortestPath(laby.mustPass[i], laby.mustPass[j]);
    std::cout << std::endl;

    std::set<int> initialSet;
    for (unsigned int i = 1; i != MUSTPASSNB; ++i)
        initialSet.insert(i);

    // laby.printGraph();

    int answer = heldkarp.findShortestTour(laby.dist, 0, initialSet) + 1;
    std::cout << std::endl << "Nombre de niveaux parcourus : " << answer << std::endl << std::endl;

    int end = 0;
    std::vector<std::string> path = {laby.entrance};
    for (std::set<int> nodes = initialSet; nodes.size() > 0; nodes.erase(end))
    {
        int bits = 0;
        for (auto it = nodes.begin(); it != nodes.end(); ++it)
            bits |= (1 << *it);
        path.push_back(laby.mustPass[end = heldkarp.previous[bits][end]]);
    }

    std::cout << "Entree du labyrinthe : " << laby.entrance << std::endl << std::endl;
    for (unsigned int i = 0; i < path.size() - 1; ++i)
        dijkstra.findShortestPath(path[i], path[i + 1], true);
    std::cout << "Sortie du labyrinthe : " << laby.exit << std::endl;

    return 0;
}
