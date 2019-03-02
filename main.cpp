#include "Laby.hh"
#include "Dijkstra.hh"
#include "Heldkarp.hh"
#include "Parser.hh"

#include <set>

Parser parser("links.txt");
Laby laby;
Dijkstra dijkstra;
Heldkarp heldkarp;

int main()
{

    for (unsigned int i = 0; i != MUSTPASSNB; ++i)
        for (unsigned int j = 0; j != MUSTPASSNB; ++j)
            laby.dist[i][j] = dijkstra.findShortestPath(laby.mustPass[i], laby.mustPass[j]);
    laby.addDummy();

    std::set<int> initialSet;
    for (unsigned int i = 1; i != MUSTPASSNB + 1; ++i)
        initialSet.insert(i);

    laby.printGraph();
/*
    int answer = heldkarp.findShortestTour(laby.dist, 0, initialSet) + 1;
    std::cout << "\rNombre de niveaux parcourus : " << answer << std::endl << std::endl;

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
    for (unsigned int i = 0; i < path.size() - 2; ++i)
    {
        /*
        std::cout << (i == 0 ? "Entree du labyrinthe" : "Point de passage") << " : " << path[i];
        for (auto it = laby.dependencies.begin(); it != laby.dependencies.end(); ++it)
        {
            if (path[i] == laby.mustPass[(*it).second])
                std::cout << " - acces ouvert par " << laby.mustPass[(*it).first];
            else if (path[i] == laby.mustPass[(*it).first])
                std::cout << " - ouvre l'acces vers " << laby.mustPass[(*it).second];
        }
        std::cout << std::endl;
        *//*
        std::cout << "Distance " << path[i] << " - " << path[i + 1] << " : " << laby.dist[parser.indexOf(laby.mustPass, path[i])][parser.indexOf(laby.mustPass, path[i + 1])] << std::endl;
        dijkstra.findShortestPath(path[i], path[i + 1], true);
    }
    std::cout << "Sortie du labyrinthe : " << laby.exit << std::endl;
*/
    return 0;
}
