#ifndef MCTS_H
#define MCTS_H

#include <iostream>
#include <cmath>
#include "node.h"
#include <memory>
#include <functional>
#include "gamerule.h"
#include <stack>
#include <cstdlib>
#include <stdexcept>
#include <utility>
#include "utility.cpp"


class GameRule;
class Node;

class MCTS
{
public:
    // constructor definition
    MCTS(GameRule *game, double C);
    // Destructor
    ~MCTS() = default;
    // copy constructor
    MCTS(const MCTS &other);
    // copy assignment operator
    MCTS &operator=(const MCTS &other);
    // move constructor
    MCTS(MCTS &&other) noexcept;
    // move assignment
    MCTS &operator=(MCTS &&other) noexcept;

    // getters
    GameRule *get_game() const;
    double get_uct_const() const;
    Node *get_root() const;
    int get_total_num_of_simulations() const;

    // methods
    static int max_depth(const Node *node);
    void serach(int num_of_simulations);
    void simulation(Node *node);

    GameRule *simulation_at_node(Node *node) const;
    std::pair<std::vector<int>, double> best_move_selection() const;

private:
    GameRule *game;
    double C;
    std::unique_ptr<Node> root;
    int total_num_of_simulations;
};

#endif