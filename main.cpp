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

void catacombesComputePathBetweenPoints(const Laby &laby, const Node &from, const Node &to, int currentState) {
    std::cout << "From (" << (int) from.getX() << ";" << (int) from.getY() << ") " << laby.getZoneName(from.getZone()) << std::endl; 
    std::cout << "To (" << (int) to.getX() << ";" << (int) to.getY() << ") " << laby.getZoneName(to.getZone()) << std::endl;
}

void catacombesDeuxPoints(const Laby &laby) {
    int optionFromX, optionFromY, optionFromState, optionFromZone, optionToX, optionToY, optionToZone;
    std::vector<uint8_t> possibleFromZones, possibleToZones;

    std::cout << "Etat actuel des boutons (de 0 a 7) : ";
    std::cin >> optionFromState;

    std::cout << "Position de depart :" << std::endl;
    std::cout << "X (de -20 a 0) : ";
    std::cin >> optionFromX;
    std::cout << "Y (de 0 a 20) : ";
    std::cin >> optionFromY;

    for (const auto &from : laby.getStatefulGraph()) {
        if (from.first.getX() == optionFromX && from.first.getY() == optionFromY && from.first.getState() == optionFromState) {
            possibleFromZones.push_back(from.first.getZone());
            optionFromZone = from.first.getZone();
        }
    }

    if (possibleFromZones.size() > 1) {
        std::cout << "Zone du niveau (" << optionFromX << ";" << optionFromY << ") :" << std::endl;
        for (unsigned int i = 0; i < possibleFromZones.size(); ++i) {
            std::cout << i + 1 << " - " << laby.getZoneName(possibleFromZones[i]) << std::endl;
        }
        std::cin >> optionFromZone;
        optionFromZone = possibleFromZones[optionFromZone - 1];
    }

    std::cout << "Position d'arrivee :" << std::endl;
    std::cout << "X (de -20 a 0) : ";
    std::cin >> optionToX;
    std::cout << "Y (de 0 a 20) : ";
    std::cin >> optionToY;

    for (const auto &from : laby.getStatefulGraph()) {
        if (from.first.getX() == optionToX && from.first.getY() == optionToY && from.first.getState() == 7) {
            possibleToZones.push_back(from.first.getZone());
            optionToZone = from.first.getZone();
        }
    }

    if (possibleToZones.size() > 1) {
        std::cout << "Zone du niveau (" << optionToX << ";" << optionToY << ") :" << std::endl;
        for (unsigned int i = 0; i < possibleToZones.size(); ++i) {
            std::cout << i + 1 << " - " << laby.getZoneName(possibleToZones[i]) << std::endl;
        }
        std::cin >> optionToZone;
        optionToZone = possibleToZones[optionToZone - 1];
    }

    catacombesComputePathBetweenPoints(laby, Node(optionFromX, optionFromY, optionFromZone), Node(optionToX, optionToY, optionToZone), optionFromState);
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
            catacombesDeuxPoints(laby);
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
