#include "node.h"

// constructor implemenrtation
Node::Node(std::vector<int> move, Node *parent, double C) : move{move}, parent{parent}, C{C}, num_wins{0}, num_simulations{0}, is_terminal{false} {};

// destructor implementation
Node::~Node()
{
    children.clear();
}

// copy constructor implementation
Node::Node(const Node &other) : move{other.move}, parent{other.parent}, C{other.C},
                                num_wins{other.num_wins}, num_simulations{other.num_simulations},
                                children{}, is_terminal{other.is_terminal}
{
    for (auto &child : other.children)
    {
        children.emplace_back(new Node(*child));
    }
    for (auto &child : children)
    {
        child->parent = this;
    }
}

// copy assignment implementation
Node &Node::operator=(const Node &other)
{
    if (this != &other)
    {
        children.clear();

        move = other.move;
        C = other.C;
        num_wins = other.num_wins;
        num_simulations = other.num_simulations;
        is_terminal = other.is_terminal;

        for (const auto &child : other.children)
        {
            children.emplace_back(std::make_unique<Node>(*child));
        }
        for (auto &child : children)
        {
            child->parent = this;
        }
    }
    return *this;
}

// move constructor implementation

Node::Node(Node &&other) : move{std::move(other.move)}, parent{other.parent},
                           C{other.C}, num_wins{other.num_wins}, num_simulations{other.num_simulations}, children{std::move(other.children)}, is_terminal{other.is_terminal}
{
    other.parent = nullptr;
    other.num_wins = 0;
    other.num_simulations = 0;
    other.is_terminal = false;
}

// move assignment operator
Node &Node::operator=(Node &&other) noexcept
{
    if (this != &other)
    {
        children.clear();

        move = std::move(other.move);
        parent = other.parent;
        C = other.C;
        num_wins = other.num_wins;
        num_simulations = other.num_simulations;
        is_terminal = other.is_terminal;

        children = std::move(other.children);
        for (auto &child : children)
        {
            child->parent = this;
        }

        other.parent = nullptr;
        other.num_wins = 0;
        other.num_simulations = 0;
        other.is_terminal = false;
    }
    return *this;
}

bool Node::is_leaf() const
{
    return children.empty();
}

bool Node::is_new() const
{
    return num_simulations == 0;
}

double Node::get_uct() const
{
    if (!parent || parent->num_simulations == 0)
    {
        throw "UCT_ERROR";
    }
    if (num_simulations == 0)
    {
        return INFINITY;
    }
    return (static_cast<double>(num_wins) / num_simulations) +
           sqrt((C * log(parent->num_simulations)) / num_simulations);
}

double Node::get_reward() const
{
    return (num_simulations > 0) ? static_cast<double>(num_wins) / num_simulations : 0.0;
}

Node *Node::get_child_with_max_uct() const
{
    if (children.empty())
    {
        return nullptr;
    }

    Node *child_with_max_uct = children.front().get();
    double max_uct_value = child_with_max_uct->get_uct();

    for (const auto &child : children)
    {
        double uct_value = child->get_uct();
        if (uct_value > max_uct_value)
        {
            child_with_max_uct = child.get();
            max_uct_value = uct_value;
        }
    }
    return child_with_max_uct;
}

Node *Node::get_child_with_max_reward() const
{
    if (children.empty())
    {
        return nullptr;
    }

    Node *child_with_max_reward = children.front().get();
    double max_reward = child_with_max_reward->get_reward();

    for (const auto &child : children)
    {
        double reward_value = child->get_reward();
        if (reward_value > max_reward)
        {
            child_with_max_reward = child.get();
            max_reward = reward_value;
        }
    }
    return child_with_max_reward;
}

Node *Node::get_child_with_max_simulation() const
{
    if (children.empty())
    {
        return nullptr;
    }

    Node *child_with_max_simulations = children.front().get(); // Get raw pointer from unique_ptr
    int max_simulation_value = child_with_max_simulations->num_simulations;

    for (const auto &child : children)
    {
        int simulation_value = child->num_simulations;
        if (simulation_value > max_simulation_value)
        {
            child_with_max_simulations = child.get(); // Get raw pointer from unique_ptr
            max_simulation_value = simulation_value;
        }
    }
    return child_with_max_simulations;
}

Node *Node::get_parent() const
{
    return parent;
}

const std::vector<int> &Node::get_move() const
{
    return move;
}

std::vector<Node *> Node::get_children() const
{
    std::vector<Node *> children_ptrs;
    for (const auto &child : children)
    {
        children_ptrs.emplace_back(child.get());
    }
    return children_ptrs;
}

void Node::add_child(Node *child_node)
{
    children.emplace_back(child_node);
}

void Node::print_children() const
{
    for (size_t i = 0; i < children.size(); ++i)
    {
        std::cout << "children[" << i << "].move: " << children[i]->move[0] << ", " << children[i]->move[1] << std::endl;
    }
}
