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
    return this->zone;
}

Cost::Cost() : distance(0), death(0) {}

Cost::Cost(uint16_t distance, uint16_t death) : distance(distance), death(death) {}

uint16_t Cost::getDistance() const {
    return this->distance;
}

uint16_t Cost::getDeath() const {
    return this->death;
}

Cost Cost::operator+(const Cost & rhs) const {
    uint32_t distance = this->distance + rhs.distance;
    uint32_t death = this->death + rhs.death;
    return Cost(distance > UINT16_MAX ? UINT16_MAX : distance, death > UINT16_MAX ? UINT16_MAX : death);
}

const Cost Cost::MAX = Cost(UINT16_MAX, UINT16_MAX);

Link::Link(Node from, Node to, Cost weight, uint16_t comment): from(from), to(to), weight(weight), comment(comment) {}

Node Link::getFrom() const {
    return this->from;
}

Node Link::getTo() const {
    return this->to;
}

Cost Link::getWeight() const {
    return this->weight;
}

ConditionalLink::ConditionalLink(Node from, Node to, Cost weight, uint8_t condition, uint16_t comment): from(from), to(to), weight(weight), condition(condition), comment(comment) {}

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

Laby::Laby(){}

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

Node Laby::makeNode(int8_t x, int8_t y, const std::string& zone) {
    uint8_t zoneIndex = this->allocZoneName(zone);
    return Node(x, y, zoneIndex);
}

void Laby::addLink(Node from, Node to, Cost weight, uint8_t condition, const std::string& comment)
{
    uint16_t commentIndex = this->allocLinkComment(comment);
    if (this->graph.count(to) == 0) {
        this->graph[to] = std::vector<ConditionalLink>();
    }
    ConditionalLink link(from, to, weight, condition, commentIndex);
    this->graph[from].push_back(link);
}

void Laby::addMustPass(Node mustpass)
{
    this->mustPass.push_back(mustpass);
}

ConditionalLink Laby::findLink(Node from, Node to) const
{
    for (const auto &value : this->graph.at(from)) {
        if (value.to == to) {
            return value;
        }
    }
    std::string message("LinkNotFound");
    throw message;
}

void Laby::printGraph() const
{
    std::cout << "List of nodes and links:" << std::endl;
    for (const auto & [from, outLinks] : this->graph)
    {
        std::cout << this->formatNode(from) << std::endl;
        for (const auto & link : outLinks) {
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

std::string Laby::formatNode(const Node & node) const {
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

const std::map<Node, std::vector<ConditionalLink>> & Laby::getGraph() const {
    return this->graph;
}
