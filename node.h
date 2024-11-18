#ifndef NODE_H
#define NODE_H

#include <cmath>
#include <iostream>
#include <vector>
#include <memory>

class Node
{
public:
    // constructor defination
    Node(std::vector<int> move, Node *parent, double C);
    // destructor
    ~Node();
    // copy constructor
    Node(const Node &other);
    //copy assignment operator
    Node& operator=(const Node& other);
    //move constructor
    Node(Node&& other) noexcept;
    //move assignment operator
    Node& operator=(Node&& other) noexcept;

    //getters
    double get_uct() const;
    double get_reward() const;
    Node* get_child_with_max_uct() const;
    Node* get_child_with_max_reward() const;
    Node* get_child_with_max_simulation() const;
    Node* get_parent() const;
    const std::vector<int>& get_move() const;
    std::vector<Node*> get_children() const;

    void add_child(Node* child_node);
    void print_children() const;

    bool is_leaf() const;
    bool is_new() const;

private:
    std::vector<int> move;
    Node *parent;
    double C; // UCT const
    int num_wins;
    int num_simulations;
    std::vector<std::unique_ptr<Node>> children;
    bool is_terminal;

    friend class MCTS;
};

#endif