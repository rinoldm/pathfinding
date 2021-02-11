#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Laby;
class StatefulNode;

/**
 * A Node (without any state information)
 */
class Node
{
private:
    friend class Laby;
    friend class StatefulNode;

    int8_t x;
    int8_t y;
    /**
     * Index for the zone name
     */
    uint8_t zone;

public:
    int8_t getX() const;
    int8_t getY() const;
    uint8_t getZone() const;
    StatefulNode withState(uint8_t state) const;

    bool operator==(const Node& rhs) const;
    bool operator!=(const Node& rhs) const;
    bool operator<(const Node& rhs) const;
    bool operator>(const Node& rhs) const;
    bool operator<=(const Node& rhs) const;
    bool operator>=(const Node& rhs) const;

    Node(int8_t x, int8_t y, uint8_t zone);
};

/**
 * A Node with state information attached to it.
 */
class StatefulNode
{
private:
    friend class Laby;
    friend class Node;

    int8_t x;
    int8_t y;
    /**
     * Index for the zone name
     */
    uint8_t zone;
    // Mask describing the state
    // Bits:
    // 0: indicates if (-4;5)_boutons is enabled
    // 1: indicates if (-5;10)_haut is enabled
    // 2: indicates if (-9;5)_bouton is enabled
    uint8_t state;

    
public:
    int8_t getX() const;
    int8_t getY() const;
    uint8_t getZone() const;
    uint8_t getState() const;
    Node toNode() const;

    bool operator==(const StatefulNode& rhs) const;
    bool operator!=(const StatefulNode& rhs) const;
    bool operator<(const StatefulNode& rhs) const;
    bool operator>(const StatefulNode& rhs) const;
    bool operator<=(const StatefulNode& rhs) const;
    bool operator>=(const StatefulNode& rhs) const;

    StatefulNode(int8_t x, int8_t y, uint8_t zone, uint8_t state);
};

/**
 * The cost of using a link (or multiple links).
 */
class Cost
{
private:
    /**
     * Distance travelled:
     * 0 for same-level zone/state transitions
     * 1 for level changes
     */
    uint16_t distance;

    /**
     * Deaths required:
     * 0 for non-death transition
     * 1 for transitions requiring the player to die
     */
    uint16_t death;
public:
    Cost();
    Cost(uint16_t distance, uint16_t death);
    uint16_t getDistance() const;
    uint16_t getDeath() const;

    bool operator==(const Cost& rhs) const;
    bool operator!=(const Cost& rhs) const;
    bool operator<(const Cost& rhs) const;
    bool operator>(const Cost& rhs) const;
    bool operator<=(const Cost& rhs) const;
    bool operator>=(const Cost& rhs) const;
    Cost operator+(const Cost& rhs) const;

    static const Cost MAX;
};

/**
 * Represents a link without any conditions attached to it
 */
class Link
{
private:
    friend class Laby;

    StatefulNode from;
    StatefulNode to;
    Cost weight;
    /**
     * Comment Index
     */
    uint16_t comment;

    Link(StatefulNode from, StatefulNode to, Cost weight, uint16_t comment);

public:
    StatefulNode getFrom() const;
    StatefulNode getTo() const;
    Cost getWeight() const;
};

/**
 * Represents a link that is only active if some conditions are met
 */
class ConditionalLink
{
private:
    friend class Laby;

    Node from;
    Node to;
    Cost weight;
    // mask describing the necessary conditions to use this link
    // 0 : (-4;5)_boutons disabled
    // 1 : (-4;5)_boutons enabled
    // 2 : (-5;10)_haut disabled
    // 3 : (-5;10)_haut enabled
    // 4 : (-9;5)_bouton disabled
    // 5 : (-9;5)_bouton enabled
    uint8_t condition;
    /**
     * Comment Index
     */
    uint16_t comment;

    ConditionalLink(Node from, Node to, Cost weight, uint8_t condition, uint16_t comment);

public:
    Node getFrom() const;
    Node getTo() const;
    Cost getWeight() const;
    uint8_t getCondition() const;
};

class Laby
{
private:
    std::vector<std::string> zoneNames;
    std::vector<std::string> linkComments;
    std::map<Node, std::vector<ConditionalLink>> graph;
    std::map<StatefulNode, std::vector<Link>> statefulGraph;
    std::vector<Node> mustPass;
    std::vector<Node> transitions;

    /**
     * Ensures the zone name is in the table of zone names and returns its index
     */
    uint8_t allocZoneName(const std::string& zone);

    /**
     * Ensures the comment to the link comments and returns its index
     */
    uint16_t allocLinkComment(const std::string& comment);

public:
    Node makeNode(int8_t x, int8_t y, const std::string& zone);
    void addLink(Node from, Node to, Cost weight, uint8_t condition, const std::string& comment);
    void addMustPass(Node mustpass);
    void addTransition(Node transition);
    void finalizeGraph();

    ConditionalLink findLink(Node from, Node to) const;
    Link findLink(StatefulNode from, StatefulNode to) const;
    void printGraph() const;
    std::string formatNode(const Node & node) const;
    std::string formatNode(const StatefulNode & node) const;
    std::string getZoneName(const Node &node) const;
    std::string getZoneName(const StatefulNode &node) const;
    std::string getZoneName(uint8_t zoneIndex) const;
    std::string getLinkComment(const Link & link) const;
    std::string getLinkComment(const ConditionalLink & link) const;

    const std::vector<Node>& getMustPass() const;
    const std::vector<Node>& getTransitions() const;
    const std::map<Node, std::vector<ConditionalLink>>& getGraph() const;
    const std::map<StatefulNode, std::vector<Link>>& getStatefulGraph() const;

    Laby();
};
