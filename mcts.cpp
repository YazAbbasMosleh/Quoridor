#include "mcts.h"

// constructor implementation
MCTS::MCTS(GameRule *game, double C) : game{game}, C{C}, total_num_of_simulations{0},
                                       root{std::make_unique<Node>(std::vector<int>(), nullptr, C)} {};

// copy constructor
MCTS::MCTS(const MCTS &other)
    : game{other.game}, C{other.C}, root{std::make_unique<Node>(*other.root)}, total_num_of_simulations{other.total_num_of_simulations} {}

// copy assignment operator
MCTS &MCTS::operator=(const MCTS &other)
{
    if (this == &other)
    {
        return *this;
    }
    game = other.game;
    C = other.C;
    root = std::make_unique<Node>(*other.root);
    total_num_of_simulations = other.total_num_of_simulations;
    return *this;
}

// move constructor
MCTS::MCTS(MCTS &&other) noexcept
    : game{other.game}, C{other.C}, root{std::move(other.root)},
      total_num_of_simulations{other.total_num_of_simulations}
{
    other.game = nullptr;
    other.C = 0.0;
    other.total_num_of_simulations = 0;
}

// Move assignment operator
MCTS &MCTS::operator=(MCTS &&other) noexcept
{
    if (this == &other)
        return *this; // Handle self-assignment
    game = other.game;
    C = other.C;
    root = std::move(other.root);
    total_num_of_simulations = other.total_num_of_simulations;

    other.game = nullptr;
    other.C = 0.0;
    other.total_num_of_simulations = 0;
    return *this;
}

// implementation getters
GameRule *MCTS::get_game() const
{
    return game;
}

double MCTS::get_uct_const() const
{
    return C;
}

Node *MCTS::get_root() const
{
    return root.get();
}
int MCTS::get_total_num_of_simulations() const
{
    return total_num_of_simulations;
}

// Static method max_depth implementation
int MCTS::max_depth(const Node *node)
{
    if (!node)
    {
        return 0;
    }
    int max = 0;
    for (const auto &child : node->children)
    {
        int d = max_depth(child.get()) + 1;
        if (d > max)
        {
            max = d;
        }
    }
    return max;
}

void MCTS::serach(int num_of_simulations)
{
    const double C = get_uct_const();
    Node *current_node = get_root();
    const int total_num_of_simulations_limitation = get_total_num_of_simulations() + num_of_simulations;

    while (get_total_num_of_simulations() < total_num_of_simulations_limitation)
    {
        // selection phase
        if (current_node->is_terminal)
        {
            simulation(current_node);
            current_node = get_root();
        }
        else if (current_node->is_leaf())
        {
            if (current_node->is_new())
            {
                simulation(current_node);
                current_node = get_root();
            }
            else
            {
                // expansion phase
                GameRule *simulation_game = simulation_at_node(current_node);
                std::vector<int> move;
                Node *child_node;

                if (simulation_game->get_pawn_of_not_turn()->get_num_left_walls() > 0)
                {
                    // get valid next positions for the pawn
                    std::vector<std::pair<int, int>> next_position_vector = simulation_game->get_valid_position();

                    // create child nodes for each valid position
                    for (const auto &vec : next_position_vector)
                    {
                        move = {vec.first, vec.second - 1};
                        child_node = new Node(move, current_node, C);
                        current_node->add_child(child_node);
                    }

                    if (simulation_game->get_pawn_of_turn()->get_num_left_walls() > 0)
                    {
                        // get valid no-block next horizontal wall positions
                        std::vector<std::pair<int, int>> safe_next_horizontal_walls = simulation_game->get_safe_probable_horizontal_walls();

                        // create child nodes for horizontal walls
                        for (const auto &pos : safe_next_horizontal_walls)
                        {
                            move = {-1, pos.first, -1};
                            child_node = new Node(move, current_node, C);
                            current_node->add_child(child_node);
                        }

                        // get valid safe next vertical wall positions
                        std::vector<std::pair<int, int>> safe_next_vertical_walls = simulation_game->get_safe_probable_vertical_walls();

                        // create child nodes for vertical walls
                        for (const auto &pos : safe_next_vertical_walls)
                        {
                            move = {-1, -1, pos.second};
                            child_node = new Node(move, current_node, C);
                            current_node->add_child(child_node);
                        }
                    }
                }
                else
                {
                    // opponent has no wall left heuristic : choose shortest path for pawn
                    std::vector<std::pair<int, int>> next_positions = AI::choose_shortest_path_next_pawn_positions(simulation_game);

                    // create child nodes for shortest path move
                    for (const auto &pos : next_positions)
                    {
                        move = {pos.first, pos.second, -1};
                        child_node = new Node(move, current_node, C);
                        current_node->add_child(child_node);
                    }
                    if (simulation_game->get_pawn_of_turn()->get_num_left_walls() > 0)
                    {
                        // opponent has no walls left heurestic: disturb opponent's path with walls
                        std::vector<std::pair<int, int>> safe_walls_disturb = simulation_game->get_safe_next_position(*simulation_game->get_pawn_of_not_turn());

                        // create child nodes for disturbing walls
                        for (const auto &pos : safe_walls_disturb)
                        {
                            move = {-1, pos.first, -1};
                            child_node = new Node(move, current_node, C);
                            current_node->add_child(child_node);
                        }

                        // create child nodes for disturbing vertical walls
                        std::vector<std::pair<int, int>> safe_vertical_walls_disturbing = simulation_game->get_safe_next_position(*simulation_game->get_pawn_of_not_turn());

                        for (const auto &pos : safe_vertical_walls_disturbing)
                        {
                            move = {-1, -1, pos.second};
                            child_node = new Node(move, current_node, C);
                            current_node->add_child(child_node);
                        }
                    }
                }

                // performing simulation on a randomly chosen node
                simulation(random_element(current_node->get_children()));

                current_node = get_root();
            }
        }
        else
        {
            // selection of child node based on uct
            current_node = current_node->get_child_with_max_uct();
        }
    }
}



std::pair<std::vector<int>, double> MCTS::best_move_selection() const
{
    Node *best_node = root->get_child_with_max_simulation();
    if (best_node)
    {
        return {best_node->get_move(), best_node->get_reward()};
    }
    else
    {
        return {{}, 0.0}; // if could not find the best move
    }
}

void MCTS::simulation(Node *node)
{
    total_num_of_simulations++;
    std::unique_ptr<GameRule> simulation_game(simulation_at_node(node));

    int node_pawn_index = simulation_game->get_pawn_index_of_not_turn();
    if (simulation_game->get_winner() != nullptr)
    {
        node->is_terminal = true;
    }

    struct PawnCache
    {
        bool is_updated;
        Position previous;
        std::vector<std::vector<Position>> next; // Assuming next is a 2D vector of Positions
        int dist_to_goal;
    };

    std::vector<PawnCache> cache_for_pawns(2, {false, Position(), std::vector<std::vector<Position>>(), 0});
    bool flag = false;

    while (simulation_game->get_winner() == nullptr)
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            if (!cache_for_pawns[i].is_updated)
            {
                auto t = AI::get_2d_array_prev_and_next_and_distance_to_goal_for(simulation_game->get_player_of_index_zero(), *simulation_game);
                cache_for_pawns[i].previous = t.first;
                cache_for_pawns[i].next = t.second;
                cache_for_pawns[i].dist_to_goal = t.third;
                cache_for_pawns[i].is_updated = true;
            }
        }

        const Player *const_pawn_of_turn = simulation_game->get_pawn_of_turn();
        Player *pawn_of_turn = const_cast<Player *>(const_pawn_of_turn);

        int pawn_index_of_turn = simulation_game->get_pawn_index_of_turn();

        if (static_cast<double>(rand()) / RAND_MAX < 0.7)
        {
            flag = false;
            std::vector<std::vector<Position>> &next_position = cache_for_pawns[pawn_index_of_turn].next;
            Position current_position = pawn_of_turn->get_position();
            Position next_pos = next_position[current_position.get_row()][current_position.get_col()];

            if (next_pos.get_row() == -1)
            {
                throw std::runtime_error("Already in goal position...");
            }

            if (AI::are_pawns_adjacent(*simulation_game))
            {
                Position next_next_pos = next_position[next_pos.get_row()][next_pos.get_col()];
                if (next_next_pos.get_row() != -1 && simulation_game->get_valid_position()[next_next_pos.get_row()][next_next_pos.get_col()])
                {
                    next_pos = next_next_pos;
                    cache_for_pawns[pawn_index_of_turn].dist_to_goal -= 2;
                }
                else
                {
                    auto next_positions = AI::choose_shortest_path_next_pawn_positions_thoroughly(*simulation_game);
                    Position _next_pos = random_element<Position>(next_positions);
                    if (_next_pos == next_pos)
                    {
                        cache_for_pawns[pawn_index_of_turn].dist_to_goal -= 1;
                    }
                    else
                    {
                        next_pos = _next_pos;
                        cache_for_pawns[pawn_index_of_turn].is_updated = false;
                    }
                }
            }
            else
            {
                cache_for_pawns[pawn_index_of_turn].dist_to_goal -= 1;
            }
            simulation_game->move_pawn(next_pos.get_row(), next_pos.get_col());
        }
        else
        {
            if (!flag && pawn_of_turn->get_num_left_walls() > 0)
            {
                auto next_move = AI::choose_probable_next_wall(*simulation_game);
                if (next_move.size() != 0)
                {
                    simulation_game->do_move(next_move);
                    cache_for_pawns[0].is_updated = false;
                    cache_for_pawns[1].is_updated = false;
                }
                else
                {
                    flag = true;
                }
            }
            else
            {
                flag = false;
                Position previous_position = cache_for_pawns[pawn_index_of_turn].previous;
                Position current_position = pawn_of_turn->get_position();
                
                // Logic for choosing previous position
                Position previous_pos;

                if (previous_position == current_position || !simulation_game->get_valid_position()[previous_position.get_row()][previous_position.get_col()])
                {
                    auto previous_positions = AI::choose_longest_path_next_pawn_positions_thoroughly(*simulation_game);
                    previous_pos = random_element<Position>(previous_positions); 
                    cache_for_pawns[pawn_index_of_turn].is_updated = false;
                }
                else
                {
                    previous_pos = previous_position;
                    cache_for_pawns[pawn_index_of_turn].dist_to_goal += 1;
                }

                simulation_game->move_pawn(previous_pos.get_row(), previous_pos.get_col());
            }
        }
    }
}


GameRule *MCTS::simulation_at_node(Node *node) const
{
    // Clone the game rule object
    GameRule *simulation_game = new GameRule(game->game_rule_clone(*game));

    std::stack<std::vector<int>> move_stack;

    Node *ancestor = node;
    while (ancestor->get_parent() != nullptr)
    {
        move_stack.push(ancestor->get_move());
        ancestor = ancestor->get_parent();
    }

    // Replay moves on the cloned game rule object
    while (!move_stack.empty())
    {
        std::vector<int> move = move_stack.top();
        move_stack.pop();

        std::vector<std::vector<int>> move_for_game_rule;
        move_for_game_rule.emplace_back(move);
        simulation_game->do_move(move_for_game_rule);
    }

    return simulation_game;
}

std::pair<std::vector<int>, double> MCTS::best_move_selection() const
{
    Node *best_node = root->get_child_with_max_simulation();
    if (best_node)
    {
        return {best_node->get_move(), best_node->get_reward()};
    }
    else
    {
        return {{}, 0.0}; // if could not find the best move
    }
}