#include "Parser.hh"
#include "Laby.hh"
#include "Dijkstra.hh"
#include "Heldkarp.hh"

int main()
{
    Parser parser("links.txt", "mustpass.txt");
    Laby laby = parser.parse();
    Dijkstra dijkstra(laby);
    Heldkarp *heldkarp = new Heldkarp(laby);

    // for each mustpass point
    for (unsigned int i = 0; i != heldkarp->mustPass.size(); ++i)
    {
        // find the shortest path between the mustpass point and every other node in the labyrinth
        dijkstra.findShortestPath(heldkarp->mustPass[i]);

        // for each mustpass point, store its shortest path with every other mustpass point
        for (unsigned int j = 0; j != heldkarp->mustPass.size(); ++j) {
            heldkarp->dist[i][j] = dijkstra.dist[heldkarp->mustPass[i]][heldkarp->mustPass[j]];
        }
    }
    std::cout << std::endl;

    // laby.printGraph();
    // we get a new matrix representing a graph between all mustpass points with their shortest distances
    heldkarp->printMatrix();

    // now we need to find the shortest tour through all the mustpass points
    Cost answer = heldkarp->findShortestTour(0, (1 << heldkarp->mustPass.size()) - 1) + Cost(1, 0);
    std::cout << std::endl << "Nombre de niveaux parcourus : " << answer.getDistance() << std::endl << std::endl;

    std::vector<Node> path = {heldkarp->mustPass.front()};
    for (int nodeBits = (1 << heldkarp->mustPass.size()) - 1, end = 0; nodeBits != 0; nodeBits &= ~(1 << end)) {
        path.push_back(heldkarp->mustPass[end = heldkarp->previous[nodeBits][end]]);
    }

    std::cout << "Entree du labyrinthe : " << laby.formatNode(heldkarp->mustPass.front()) << std::endl << std::endl;
    for (unsigned int i = 1; i < path.size() - 1; ++i) {
        dijkstra.printShortestPath(*heldkarp, path[i], path[i + 1]);
    }
    std::cout << "Sortie du labyrinthe : " << laby.formatNode(heldkarp->mustPass.back()) << std::endl;

    return 0;
}
