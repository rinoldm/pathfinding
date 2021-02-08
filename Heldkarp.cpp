#include "Heldkarp.hh"

Heldkarp::Heldkarp(const Laby &laby, const std::map<StatefulNode, std::map<StatefulNode, Cost>> & dist) : laby(laby)
{
    std::vector<StatefulNode> statefulMustpass;
    for (const Node &mp : laby.getMustPass()) {
        for (uint8_t state = 0; state < 8; ++state) {
            statefulMustpass.push_back(mp.withState(state));
        }
    }

    size_t stNodeId = 0;
    for (const auto & from : statefulMustpass) {
        for (const auto & to : statefulMustpass) {
            this->costs.push_back(dist.at(from).at(to));
        }
        this->statefulNodeToId[from] = stNodeId++;
    }
    this->NODE_COUNT = laby.getMustPass().size();
    this->STATEFUL_NODE_COUNT = stNodeId;

    for (size_t visitedMask = 0; visitedMask < (1 << this->NODE_COUNT); visitedMask++) {
        for (size_t lastStNodeId = 0; lastStNodeId < this->STATEFUL_NODE_COUNT; lastStNodeId++) {
            this->tourCosts.push_back(Cost());
            this->previous.push_back(0);
        }
    }
}

Cost Heldkarp::getCost(const StatefulNode &from, const StatefulNode &to) {
    return this->getCost(this->statefulNodeToId[from], this->statefulNodeToId[to]);
}

Cost Heldkarp::getCost(size_t from, size_t to) {
    return this->costs[from * this->STATEFUL_NODE_COUNT + to];
}

Cost Heldkarp::getTourCost(uint32_t visitedSet, size_t lastStNodeId) {
    return this->tourCosts[visitedSet * this->STATEFUL_NODE_COUNT + lastStNodeId];
}

void Heldkarp::setTourCost(uint32_t visitedSet, size_t lastStNodeId, Cost c) {
    this->tourCosts[visitedSet * this->STATEFUL_NODE_COUNT + lastStNodeId] = c;
}

void Heldkarp::setPrevious(uint32_t visitedSet, size_t lastStNodeId, size_t prevId) {
    this->previous[visitedSet * this->STATEFUL_NODE_COUNT + lastStNodeId] = prevId;
}

Cost Heldkarp::findShortestTour()
{
    size_t exitNodeId = this->NODE_COUNT - 1;
    size_t exitId = exitNodeId * 8 + 0b111;
    uint32_t allVisitedMask = (1 << this->NODE_COUNT) - 1;
    uint32_t allVisitedMaskExceptExit = allVisitedMask & ~(1 << exitNodeId);
    return this->innerFindShortestTour(allVisitedMaskExceptExit, exitId);
}

// [{0, 1, 2, 3, 4,  }; 5x]

// [{0, 1, 2,  ,  ,  }; 4a]

// [{0, 1,  ,  ,  ,  }; 2a] + (2a -> 4a)
// [{0, 1,  ,  ,  ,  }; 2_] + (2_ -> 4a)
// [{0,  , 2,  ,  ,  }; 1a] + (1a -> 4a)
// [{0,  , 2,  ,  ,  }; 1_] + (1_ -> 4a)
// [{ , 1, 2,  ,  ,  }; 0a] + (0a -> 4a)
// [{ , 1, 2,  ,  ,  }; 0_] + (0_ -> 4a)

// [{0, 1, 2,  ,  }; 4A]
// [{0,  ,  ,  ,  }; 3A]

// [{}, 15[0]] -> MAX
// [{}, 12[0]] -> Ma
// [{}, 15[3]]
// [{}, 0[0]] -> 0
// [{}, 0[7]] -> dijkstra: 0[0] -> 0[7]

// Computes the shortest tour
//
// - visitedSet: subSet of non-stateful nodes to visit
// - lastStNodeId: id the last stateful node to visit
Cost Heldkarp::innerFindShortestTour(uint32_t visitedSet, size_t lastStNodeId)
{
    Cost oldCost = this->getTourCost(visitedSet, lastStNodeId);
    if (oldCost != Cost()) {
        return oldCost;
    }

    if (visitedSet == 0) {
        bool isEntryNode = (lastStNodeId >> 3) == 0;
        Cost c;
        if (isEntryNode) {
            c = this->getCost(0, lastStNodeId);
        } else {
            c = Cost::MAX;
        }
        this->setTourCost(visitedSet, lastStNodeId, c);
        return c;
    }

    Cost minFound = Cost::MAX;
    for (size_t prevNodeIdx = 0; prevNodeIdx < this->NODE_COUNT; prevNodeIdx++)
    {
        if (!(visitedSet & (1 << prevNodeIdx))) {
            continue;
        }
        for (uint8_t prevState = 0; prevState < 8; prevState++) {
            size_t prevStNodeId = prevNodeIdx * 8 + prevState;
            uint32_t prevVisitedSet = visitedSet & ~(1 << prevNodeIdx);
            Cost prevTourCost = this->innerFindShortestTour(prevVisitedSet, prevStNodeId);
            Cost newCost = prevTourCost + this->getCost(prevStNodeId, lastStNodeId);
            if (newCost < minFound) {
                minFound = newCost;
                this->setPrevious(visitedSet, lastStNodeId, prevStNodeId);
            }
        }
    }

    this->setTourCost(visitedSet, lastStNodeId, minFound);
    return minFound;
}
