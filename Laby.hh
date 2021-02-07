#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Laby;

/**
 * A Node (without any state information)
 */
class Node
{
private:
    friend class Laby;

    int8_t x;
    int8_t y;
    /**
     * Index for the zone name
     */
    uint8_t zone;

    Node(int8_t x, int8_t y, uint8_t zone);

public:
    int8_t getX() const;
    int8_t getY() const;
    uint8_t getZone() const;

    auto operator<=>(const Node&) const = default;
};

/**
 * A Node with state information attached to it.
 */
class StatefulNode
{
private:
    friend class Laby;

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

    StatefulNode(int8_t x, int8_t y, uint8_t zone, uint8_t state);

public:
    int8_t getX() const;
    int8_t getY() const;
    uint8_t getZone() const;
    uint8_t getState() const;
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

    auto operator<=>(const Cost&) const = default;
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

    Node from;
    Node to;
    Cost weight;
    /**
     * Comment Index
     */
    uint16_t comment;

    Link(Node from, Node to, Cost weight, uint16_t comment);

public:
    Node getFrom() const;
    Node getTo() const;
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
    std::vector<Node> mustPass;

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

    ConditionalLink findLink(Node from, Node to) const;
    void printGraph() const;
    std::string formatNode(const Node & node) const;
    std::string getLinkComment(const Link & link) const;
    std::string getLinkComment(const ConditionalLink & link) const;

    const std::vector<Node>& getMustPass() const;
    const std::map<Node, std::vector<ConditionalLink>>& getGraph() const;

    Laby();
};
