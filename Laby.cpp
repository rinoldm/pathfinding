#include "Laby.hh"

Node::Node(int8_t x, int8_t y, uint8_t zone) : x(x), y(y), zone(zone) {}

int8_t Node::getX() const {
    return this->x;
}

int8_t Node::getY() const {
    return this->y;
}

uint8_t Node::getZone() const {
    return this->zone;
}

StatefulNode Node::withState(uint8_t state) const {
    return StatefulNode(this->x, this->y, this->zone, state);
}

bool Node::operator==(const Node &rhs) const {
    return this->x == rhs.x && this->y == rhs.y && this->zone == rhs.zone;
}

bool Node::operator!=(const Node &rhs) const {
    return !(*this == rhs);
}

bool Node::operator<(const Node &rhs) const {
    if (this->x != rhs.x) {
        return this->x < rhs.x;
    }
    else if (this->y != rhs.y) {
        return this->y < rhs.y;
    }
    else {
        return this->zone < rhs.zone;
    }
}

bool Node::operator>(const Node &rhs) const {
    return rhs < *this;
}

bool Node::operator<=(const Node &rhs) const {
    return !(*this > rhs);
}

bool Node::operator>=(const Node &rhs) const {
    return !(*this < rhs);
}

StatefulNode::StatefulNode(int8_t x, int8_t y, uint8_t zone, uint8_t state) : x(x), y(y), zone(zone), state(state) {}

int8_t StatefulNode::getX() const {
    return this->x;
}

int8_t StatefulNode::getY() const {
    return this->y;
}

uint8_t StatefulNode::getZone() const {
    return this->zone;
}

uint8_t StatefulNode::getState() const {
    return this->state;
}

Node StatefulNode::toNode() const {
    return Node(this->x, this->y, this->zone);
}

bool StatefulNode::operator==(const StatefulNode &rhs) const {
    return this->x == rhs.x && this->y == rhs.y && this->zone == rhs.zone && this->state == rhs.state;
}

bool StatefulNode::operator!=(const StatefulNode &rhs) const {
    return !(*this == rhs);
}

bool StatefulNode::operator<(const StatefulNode &rhs) const {
    if (this->x != rhs.x) {
        return this->x < rhs.x;
    }
    else if (this->y != rhs.y) {
        return this->y < rhs.y;
    }
    else if (this->zone != rhs.zone) {
        return this->zone < rhs.zone;
    }
    else {
        return this->state < rhs.state;
    }
}

bool StatefulNode::operator>(const StatefulNode &rhs) const {
    return rhs < *this;
}

bool StatefulNode::operator<=(const StatefulNode &rhs) const {
    return !(*this > rhs);
}

bool StatefulNode::operator>=(const StatefulNode &rhs) const {
    return !(*this < rhs);
}

Cost::Cost() : distance(0), death(0) {}

Cost::Cost(uint16_t distance, uint16_t death) : distance(distance), death(death) {}

uint16_t Cost::getDistance() const {
    return this->distance;
}

uint16_t Cost::getDeath() const {
    return this->death;
}

Cost Cost::operator+(const Cost &rhs) const {
    uint32_t distance = this->distance + rhs.distance;
    uint32_t death = this->death + rhs.death;
    return Cost(distance > UINT16_MAX ? UINT16_MAX : distance, death > UINT16_MAX ? UINT16_MAX : death);
}

const Cost Cost::MAX = Cost(UINT16_MAX, UINT16_MAX);

bool Cost::operator==(const Cost &rhs) const {
    return this->distance == rhs.distance && this->death == rhs.death;
}

bool Cost::operator!=(const Cost &rhs) const {
    return !(*this == rhs);
}

bool Cost::operator<(const Cost &rhs) const {
    if (this->distance != rhs.distance) {
        return this->distance < rhs.distance;
    }
    else {
        return this->death < rhs.death;
    }
}

bool Cost::operator>(const Cost &rhs) const {
    return rhs < *this;
}

bool Cost::operator<=(const Cost &rhs) const {
    return !(*this > rhs);
}

bool Cost::operator>=(const Cost &rhs) const {
    return !(*this < rhs);
}

Link::Link(StatefulNode from, StatefulNode to, Cost weight, uint16_t comment) : from(from), to(to), weight(weight), comment(comment) {}

StatefulNode Link::getFrom() const {
    return this->from;
}

StatefulNode Link::getTo() const {
    return this->to;
}

Cost Link::getWeight() const {
    return this->weight;
}

ConditionalLink::ConditionalLink(Node from, Node to, Cost weight, uint8_t condition, uint16_t comment) : from(from), to(to), weight(weight), condition(condition), comment(comment) {}

Node ConditionalLink::getFrom() const {
    return this->from;
}

Node ConditionalLink::getTo() const {
    return this->to;
}

Cost ConditionalLink::getWeight() const {
    return this->weight;
}

uint8_t ConditionalLink::getCondition() const {
    return this->condition;
}

Laby::Laby() {}

uint8_t Laby::allocZoneName(const std::string &zone) {
    uint8_t zoneIndex = 0;
    while (zoneIndex < 255 && zoneIndex < this->zoneNames.size() && this->zoneNames[zoneIndex] != zone) {
        zoneIndex += 1;
    }
    if (zoneIndex == this->zoneNames.size()) {
        this->zoneNames.push_back(zone);
    }
    if (zoneIndex >= this->zoneNames.size() || this->zoneNames[zoneIndex] != zone) {
        std::string message("FailedToAllocateZoneName");
        throw message;
    }
    return zoneIndex;
}

uint16_t Laby::allocLinkComment(const std::string &comment) {
    uint16_t commentIndex = this->linkComments.size();
    this->linkComments.push_back(comment);
    if (commentIndex >= this->linkComments.size() || this->linkComments[commentIndex] != comment) {
        std::string message("FailedToAllocateLinkComment");
        throw message;
    }
    return commentIndex;
}

Node Laby::makeNode(int8_t x, int8_t y, const std::string &zone) {
    uint8_t zoneIndex = this->allocZoneName(zone);
    return Node(x, y, zoneIndex);
}

void Laby::addLink(Node from, Node to, Cost weight, uint8_t condition, const std::string &comment) {
    uint16_t commentIndex = this->allocLinkComment(comment);
    if (this->graph.count(to) == 0) {
        this->graph[to] = std::vector<ConditionalLink>();
    }
    ConditionalLink link(from, to, weight, condition, commentIndex);
    this->graph[from].push_back(link);
}

void Laby::addMustPass(Node mustpass) {
    this->mustPass.push_back(mustpass);
}

void Laby::addTransition(Node transition) {
    this->transitions.push_back(transition);
}

void Laby::finalizeGraph() {
    for (uint8_t state = 0; state < 8; ++state) {
        uint8_t stateMask = 0;
        stateMask |= 1 << ((state & (1 << 0)) ? 1 : 0);
        stateMask |= 1 << ((state & (1 << 1)) ? 3 : 2);
        stateMask |= 1 << ((state & (1 << 2)) ? 5 : 4);

        for (const auto &[from, outLinks] : this->graph) {
            StatefulNode sFrom = from.withState(state);
            for (const auto &cLink : outLinks) {
                StatefulNode sTo = cLink.to.withState(state);
                if (this->statefulGraph.count(sTo) == 0) {
                    this->statefulGraph[sTo] = std::vector<Link>();
                }
                uint8_t conditionResult = stateMask & cLink.condition;
                bool success = (0b11 & conditionResult) && ((0b11 << 2) & conditionResult) && ((0b11 << 4) & conditionResult);
                if (success) {
                    Link link(sFrom, sTo, cLink.weight, cLink.comment);
                    this->statefulGraph[sFrom].push_back(link);
                }
            }
        }
    }

    for (uint8_t transitionNb = 0; transitionNb < this->transitions.size(); ++transitionNb) {
        for (uint8_t i = 0; i < this->transitions.size() + 1; ++i) {
            uint8_t highMask = (~0) << transitionNb;
            uint8_t lowMask = ~highMask;
            uint8_t high = highMask & i;
            uint8_t low = lowMask & i;

            StatefulNode sFrom = this->transitions[transitionNb].withState((high << 1) | (0 << transitionNb) | low);
            StatefulNode sTo   = this->transitions[transitionNb].withState((high << 1) | (1 << transitionNb) | low);
            Link link(sFrom, sTo, Cost(), this->allocLinkComment(std::string("presser le bouton")));
            this->statefulGraph[sFrom].push_back(link);
        }
    }
}

ConditionalLink Laby::findLink(Node from, Node to) const {
    for (const auto &value : this->graph.at(from)) {
        if (value.to == to) {
            return value;
        }
    }
    std::string message("LinkNotFound");
    throw message;
}

Link Laby::findLink(StatefulNode from, StatefulNode to) const {
    for (const auto &value : this->statefulGraph.at(from)) {
        if (value.to == to) {
            return value;
        }
    }
    std::string message("LinkNotFound");
    throw message;
}

void Laby::printGraph() const {
    std::cout << "List of nodes and links:" << std::endl;
    for (const auto &[from, outLinks] : this->graph) {
        std::cout << this->formatNode(from) << std::endl;
        for (const auto &link : outLinks) {
            std::cout
                << "  -> " << this->formatNode(link.getTo())
                << " " << link.getWeight().getDistance()
                << " " << link.getWeight().getDeath()
                << " " << this->getLinkComment(link)
                << std::endl;
        }
    }
    std::cout << std::endl;
}

std::string Laby::formatNode(const Node &node) const {
    std::string x = std::to_string(node.getX());
    std::string y = std::to_string(node.getY());
    uint8_t zoneIndex = node.getZone();
    std::string zone = this->zoneNames[zoneIndex];
    std::stringstream ss;
    ss << '(' << x << ';' << y << ')';
    if (!zone.empty()) {
        ss << '_' << zone;
    }
    return ss.str();
}

std::string Laby::formatNode(const StatefulNode &node) const {
    std::string x = std::to_string(node.getX());
    std::string y = std::to_string(node.getY());
    uint8_t zoneIndex = node.getZone();
    std::string zone = this->zoneNames[zoneIndex];
    std::string state = std::to_string(node.getState());
    std::stringstream ss;
    ss << '(' << x << ';' << y << ')';
    if (!zone.empty()) {
        ss << '_' << zone;
    }
    ss << '[' << state << ']';
    return ss.str();
}

std::string Laby::getZoneName(const Node &node) const {
    uint8_t zoneIndex = node.getZone();
    return this->zoneNames[zoneIndex];
}

std::string Laby::getZoneName(const StatefulNode &node) const {
    uint8_t zoneIndex = node.getZone();
    return this->zoneNames[zoneIndex];
}

std::string Laby::getZoneName(uint8_t zoneIndex) const {
    return this->zoneNames[zoneIndex];
}

std::string Laby::getLinkComment(const Link &link) const {
    uint16_t commentIndex = link.comment;
    return this->linkComments[commentIndex];
}

std::string Laby::getLinkComment(const ConditionalLink &link) const {
    uint16_t commentIndex = link.comment;
    return this->linkComments[commentIndex];
}

const std::vector<Node> &Laby::getMustPass() const {
    return this->mustPass;
}

const std::vector<Node> &Laby::getTransitions() const {
    return this->transitions;
}

const std::map<Node, std::vector<ConditionalLink>> &Laby::getGraph() const {
    return this->graph;
}

const std::map<StatefulNode, std::vector<Link>> &Laby::getStatefulGraph() const {
    return this->statefulGraph;
}
