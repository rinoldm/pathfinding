#include "Parser.hh"
#include "Laby.hh"
#include "Dijkstra.hh"
#include "Heldkarp.hh"

void printDistanceMatrix(const Laby &laby, const std::map<StatefulNode, std::map<StatefulNode, Cost>> &dist) {
    for (const auto &[from, outLinks] : dist) {
        std::cout << laby.formatNode(from) << ":";
        for (const auto &[to, cost] : outLinks) {
            std::cout << " " << cost.getDistance();
        }
        std::cout << std::endl;
    }
}

int main() {
    Parser parser("links.txt", "nodes.txt");
    Laby laby = parser.parse();

    // laby.printGraph();

    const std::vector<Node> &mustpass = laby.getMustPass();
    std::vector<StatefulNode> statefulMustpass;
    for (const Node &mp : mustpass) {
        for (uint8_t state = 0; state < 8; ++state) {
            statefulMustpass.push_back(mp.withState(state));
        }
    }

    Dijkstra dijkstra(laby);
    std::map<StatefulNode, std::map<StatefulNode, Cost>> dist = dijkstra.shortestAllPairs(statefulMustpass);

    // we get a new matrix representing a graph between all mustpass points with their shortest distances
    printDistanceMatrix(laby, dist);

    std::cout << std::endl;

    // now we need to find the shortest tour through all the mustpass points
    Heldkarp *heldkarp = new Heldkarp(laby, dist);
    Cost answer = heldkarp->findShortestTour();

    std::cout << std::endl << "Nombre de niveaux parcourus : " << answer.getDistance() << ", " << answer.getDeath() << std::endl << std::endl;

//    Cost answer = heldkarp->findShortestTour(0, (1 << heldkarp->mustPass.size()) - 1) + Cost(1, 0);

    /*

    std::vector<Node> path = {heldkarp->mustPass.front()};
    for (int nodeBits = (1 << heldkarp->mustPass.size()) - 1, end = 0; nodeBits != 0; nodeBits &= ~(1 << end)) {
        path.push_back(heldkarp->mustPass[end = heldkarp->previous[nodeBits][end]]);
    }

    std::cout << "Entree du labyrinthe : " << laby.formatNode(heldkarp->mustPass.front()) << std::endl << std::endl;
    for (unsigned int i = 1; i < path.size() - 1; ++i) {
        dijkstra.printShortestPath(*heldkarp, path[i], path[i + 1]);
    }
    std::cout << "Sortie du labyrinthe : " << laby.formatNode(heldkarp->mustPass.back()) << std::endl;
*/
    return 0;
}
