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

void catacombesOptimalPath(const Laby &laby) {
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
    // printDistanceMatrix(laby, dist);

    // now we need to find the shortest tour through all the mustpass points
    Heldkarp *heldkarp = new Heldkarp(laby, dist);
    auto [answer, path] = heldkarp->findShortestTour();

    std::cout << std::endl << "Nombre de niveaux parcourus : " << answer.getDistance() << ", " << answer.getDeath() << std::endl << std::endl;

    std::cout << ">> " << laby.formatNode(path[0]) << std::endl << std::endl;

    for (size_t i = 1; i < path.size(); i++) {
        StatefulNode from = path[i - 1];
        StatefulNode to = path[i];

        std::deque<StatefulNode> subPath =  dijkstra.getPath(from, to);

        for (size_t j = 0; j < subPath.size(); j++) {
            StatefulNode subTo = subPath[j];
            if (j > 0) {
                StatefulNode subFrom = subPath[j - 1];
                Link l = laby.findLink(subFrom, subTo);
                std::cout << "    | " << laby.getLinkComment(l) << std::endl;
            }
            std::cout << "    " << laby.formatNode(subTo) << std::endl;
        }

        std::cout << std::endl;
        std::cout << ">> " << laby.formatNode(to) << std::endl << std::endl;
    }
}

void menuCatacombes() {
    Parser parser("catacombes_links.txt", "catacombes_nodes.txt");
    Laby laby = parser.parse();
    // laby.printGraph();

    std::cout << "Labyrinthe : Les catacombes de Tuberculoz" << std::endl;
    std::cout << "1 - Chemin optimal" << std::endl;
    std::cout << "2 - Chemin entre deux points" << std::endl;

    int optionChemin = 0;
    std::cin >> optionChemin;
    switch(optionChemin) {
        case 1:
            std::cout << "Chemin optimal" << std::endl;
            catacombesOptimalPath(laby);
            break;
        case 2:
            std::cout << "Chemin entre deux points" << std::endl;
            break;
        default:
            break;
    }
}

int main() {
    std::cout << "Bienvenue dans le Pathfinder. Selectionnez une des options suivantes :" << std::endl;
    std::cout << "1 - Les catacombes de Tuberculoz" << std::endl;
    std::cout << "2 - Prochain laby (TODO)" << std::endl;

    int optionLaby = 0;
    std::cin >> optionLaby;
    switch(optionLaby) {
        case 1:
            menuCatacombes();
            break;
        case 2:
            std::cout << "Prochain laby (TODO)" << std::endl;
            break;
        default:
            break;
    }
    return 0;
}
