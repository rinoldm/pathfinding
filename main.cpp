#include "Parser.hh"
#include "Laby.hh"
#include "Dijkstra.hh"
#include "Heldkarp.hh"

Heldkarp heldkarp;

int main()
{
    Parser parser("links.txt");
    Laby laby(parser);
    Dijkstra dijkstra(laby);

    for (unsigned int i = 0; i != heldkarp.mustPass.size(); ++i)
    {
        dijkstra.findShortestPath(heldkarp.mustPass[i]);
        for (unsigned int j = 0; j != heldkarp.mustPass.size(); ++j)
            heldkarp.dist[i][j] = dijkstra.dist[heldkarp.mustPass[i]][heldkarp.mustPass[j]];
    }
    std::cout << std::endl;

    // laby.printGraph();
    heldkarp.printMatrix();

    int answer = heldkarp.findShortestTour(0, (1 << heldkarp.mustPass.size()) - 1) + 1;
    std::cout << std::endl << "Nombre de niveaux parcourus : " << answer << std::endl << std::endl;

    std::vector<std::string> path = {heldkarp.mustPass.front()};
    for (int nodeBits = (1 << heldkarp.mustPass.size()) - 1, end = 0; nodeBits != 0; nodeBits &= ~(1 << end))
        path.push_back(heldkarp.mustPass[end = heldkarp.previous[nodeBits][end]]);

    std::cout << "Entree du labyrinthe : " << heldkarp.mustPass.front() << std::endl << std::endl;
    for (unsigned int i = 1; i < path.size() - 1; ++i)
        dijkstra.printShortestPath(path[i], path[i + 1]);
    std::cout << "Sortie du labyrinthe : " << heldkarp.mustPass.back() << std::endl;

    return 0;
}
