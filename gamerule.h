#ifndef GAMERULE_H
#define GAMERULE_H

#include <iostream>
#include <vector>
#include <utility>
#include "utility.cpp"
#include "position.h"
#include "player.h"
#include <memory>
#include "gameboard.h"
#include <functional>

class Player;
class Position;
class GameBoard;

class GameRule
{
    /*
    !board:
        ?game board on which the Quoridor game is played
    !winner:
        ?pawn that has won the game.
    !turn:
        ?keeps track of the current turn in the game
            *placing wall
            *moving pawn
    !valid_next_walls:
        ?valid locations for placing walls on the game board
    !_probable_next_wall:
        ?the probable locations for placing walls on the next turn
    !probable_valid_next_walls:
        ?probable wall locations that are also valid
        ? according to the valid_next_walls array
    !probable_valid_next_walls-updated:
        ? indicates whether the _probableValidNextWalls array has been updated
        ? after the last turn. If it’s false, the probable_valid_next_walls
        ? array needs to be recalculated based on the current game state.
    !open_ways:
        ?represent the open paths between adjacent squares on the game board.
        ?A value of true at a specific location in the array indicates
        ? that the path is open (not blocked by a wall).
        ? while false indicates that the path is blocked
        ? This array is updated each time a wall is placed on the board
    !valid_next_positions:
        ?represents the valid positions for the pawn to move to on the next turn.
        ? A value of true at a specific location in the array indicates
        ? that the pawn can move there, while false indicates that the
        ? pawn cannot move there. This array is updated each time a pawn is moved or a wall is placed.
    !valid_next_positions_udated:
        ?represents the valid positions for the pawn to move to on the next turn.
        ? A value of true at a specific location in the array indicates that
        ? the pawn can move there, while false indicates that the pawn cannot move there.
        ? This array is updated each time a pawn is moved or a wall is placed.
    */
public:
    // default constructor
    GameRule();
    // constructor
    GameRule(bool is_opp_first, bool flag = false);
    // destructor
    ~GameRule() = default;
    // copy constructor
    GameRule(const GameRule &other);
    // move constructor
    GameRule(GameRule &&other) noexcept;

    // move assignment operator
    GameRule &operator=(GameRule &&other) noexcept;
    // copy assignment operator
    GameRule &operator=(const GameRule &other);

    // getters
    int get_turn() const;
    const Player *get_player_of_index_zero() const;
    const Player *get_player_of_index_one() const;
    int get_pawn_index_of_turn() const;
    int get_pawn_index_of_not_turn() const;
    const Player *get_pawn_of_turn() const;
    const Player *get_pawn_of_not_turn() const;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> get_probable_valid_next_walls();
    std::vector<std::vector<bool>> get_next_positions();
    Player* get_winner() const;

    // setter
    void set_turn(int value);
    void set_valid_move(const std::vector<int> &main_move,
                        const std::vector<int> sub_move1, const std::vector<int> sub_move2);

    // methods
    bool is_valid_next_move_not_considering_other_pawn(const Position &current_position, const std::vector<int> &pawn_move);
    bool move_pawn(int row, int col, bool flag = false);
    bool is_open_way(int current_row, int current_col, const std::vector<int> &pawn_move);

    // wall strategy condition analyzing
    bool is_there_open_way(int current_row, int current_col, const std::vector<int> &pawn_move);
    bool is_adjacent_left_wall_horizontal(int row, int col);
    bool is_adjacent_right_wall_horizontal(int row, int col);
    bool is_adjacent_middle_wall_horizontal(int row, int col);
    bool is_horizontal_wall_connected(int row, int col);
    bool is_adjacent_top_wall_vertical(int row, int col);
    bool is_adjacent_bottom_wall_vertical(int row, int col);
    bool is_adjacent_middle_wall_vertical(int row, int col);
    bool is_vertical_wall_connected(int row, int col);

    // placing wall must not avoid the pawns reaching the goal row
    bool can_place_vertical_wall_safely(int row, int col);
    bool can_place_horizontal_wall_safely(int row, int col);

    bool is_valid_move(std::vector<std::vector<int>> move);

    void update_probable_next_walls_after_placing_vertical_wall(int row, int col);
    void update_probable_valid_next_wall_after_placing_horizontal_wall(int row, int col);

    /*
    !"is_path_to_goal" fucntion:
        ? to determine if there exists a path from the current position
        ? of a given pawn to its goal line on the game board.
            *visited : visietd positions in search
            *pawn_move_tuples: possible movements
    */
    bool is_path_to_goal(const Player &pawn)
    {

        std::vector<std::vector<bool>> visited(9, std::vector<bool>(9, false));
        std::vector<std::vector<int>> pawn_move_tuples = {MOVE_UP, MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN};

        // depth-first search (DFS) function
        std::function<bool(int, int, int)> depth_first_search = [&](int current_row, int current_col, int goal_row)
        {
            for (const auto &pawn_move : pawn_move_tuples)
            {
                if (is_there_open_way(current_row, current_col, pawn_move))
                {
                    int next_row = current_row + pawn_move[0];
                    int next_col = current_col + pawn_move[1];
                    if (!visited[next_row][next_col])
                    {
                        visited[next_row][next_col] = true;
                        if (next_row == goal_row)
                        {
                            return true;
                        }
                        if (depth_first_search(next_row, next_col, goal_row))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        };
        return depth_first_search(pawn.get_position().get_row(), pawn.get_position().get_col(), pawn.get_goal_row());
    }

    bool is_path_to_goal_for_both();

    /*
    ! "set_walls" funciton:
        ? The set_walls function is used to set the possible
        ? wall positions around a given pawn on a GameRule::gameRule board
            *wall_2D_arrays.first --> horizontal walls
            *wall_2D_arrays.second --> vertical walls
            *pawn --> for accessing the position of the pawn
    */

    static void GameRule::set_walls(std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> &wall_array, const Player *pawn)
    {
        int row = pawn->get_position().get_row();
        int col = pawn->get_position().get_col();
        if (row >= 1)
        {
            if (col >= 1)
            {
                wall_array.first[row - 1][col - 1] = true;
                wall_array.second[row - 1][col - 1] = true;
                if (col >= 2)
                {
                    wall_array.first[row - 1][col - 2] = true;
                }
            }
            if (col <= 7)
            {
                wall_array.first[row - 1][col] = true;
                wall_array.second[row - 1][col] = true;
                if (col <= 6)
                {
                    wall_array.first[row - 1][col + 1] = true;
                }
            }
            if (row >= 2)
            {
                if (col >= 1)
                {
                    wall_array.second[row - 2][col - 1] = true;
                }
                if (col <= 7)
                {
                    wall_array.second[row - 2][col] = true;
                }
            }
        }
        if (row <= 7)
        {
            if (col >= 1)
            {
                wall_array.first[row][col - 1] = true;
                wall_array.second[row][col - 1] = true;
                if (col >= 2)
                {
                    wall_array.first[row][col - 2] = true;
                }
            }
            if (col <= 7)
            {
                wall_array.first[row][col] = true;
                wall_array.second[row][col] = true;
                if (col <= 6)
                {
                    wall_array.first[row][col + 1] = true;
                }
            }
            if (row <= 6)
            {
                if (col >= 1)
                {
                    wall_array.second[row + 1][col - 1] = true;
                }
                if (col <= 7)
                {
                    wall_array.second[row + 1][col] = true;
                }
            }
        }
    }

    bool place_horizontal_wall(int row, int col, bool flag = false);
    bool place_vertical_wall(int row, int col, bool flag = false);
    bool do_move(const std::vector<std::vector<int>> &move, bool flag = false);

    GameRule game_rule_clone(const GameRule &game_rule);
    std::vector<std::pair<int, int>> get_valid_position() const;

    std::vector<std::pair<int, int>> get_valid_horizontal_wall();
    std::vector<std::pair<int, int>> get_valid_vertical_wall();

    std::vector<std::pair<int, int>> get_safe_probable_horizontal_walls();
    std::vector<std::pair<int, int>> get_safe_probable_vertical_walls();

    std::vector<std::pair<int , int>> get_safe_next_position(const Player &pawn) const;


private:
    std::unique_ptr<GameBoard> board;
    Player *winner;
    int turn;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> valid_next_walls;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> probable_next_walls;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> probable_valid_next_walls;
    bool is_updated_probable_valid_next_walls;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> open_ways;
    std::vector<std::vector<bool>> valid_next_positions;
    bool is_updated_valid_next_position;
};

#endif
