#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <utility>
#include "utility.cpp"
#include "position.h"

class Player
{
    /*
    !index:
        ?which Player we are dealing with(0 --> first one , 1--> second one)
    !is_opp_side:
        ?indicates which side of the game board the Player is on.
        ?If it is true, the Player is on the opponent side of the board.
        ?If it is false, the Player is on the computer side of the board.
    !is_opp_player:
        ? indicates whether the Player is controlled by a human player.
        ? If it is true, the Player is controlled by a opponent.
        ? If it is false, the Player is controlled by the computer (AI).
    !position:
        ?current position of the Player on the game board
    !goal_row:
        ?the row that the Player is trying to reach
    !num_left_walls:
        ?the number of walls that the Player has left to place on the board
    */
public:
    // default constructor
    Player() = default;
    // constructor
    Player(int pawn_index, bool is_opp_side, bool is_opp_player, bool flag = false);
    // Destructor
    ~Player() = default;
    // Copy Constructor
    Player(const Player &other);
    // Copy Assignment Operator
    Player &operator=(const Player &other);
    // Move Constructor
    Player(Player &&other) noexcept;
    // Move Assignment Operator
    Player &operator=(Player &&other) noexcept;

    // getters
    int get_pawn_index() const;
    bool get_is_opp_player() const;
    bool get_is_opp_side() const;
    Position get_position() const;
    int get_goal_row() const;
    int get_num_left_walls() const;

    // setters
    void set_pawn_index(int value);
    void set_is_opp_player(bool value);
    void set_is_opp_side(bool value);
    void set_position(Position value);
    void set_goal_row(int value);
    void set_num_left_walls(int value);

    static Player player_clone(const Player &player);

private:
    int pawn_index;
    bool is_opp_side;
    bool is_opp_player;
    Position position;
    int goal_row;
    int num_left_walls;
};

#endif