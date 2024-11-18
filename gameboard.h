#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <iostream>
#include <vector>
#include <utility>
#include "utility.cpp"
#include "position.h"
#include "player.h"

class Player;
class Position;

class GameBoard
{
    /*
    !pawns:
        ?vector to store pawns
    !walls:
        ?structure to store walls
        !horizontal:
            *horizontal walls
        !vertical:
            *vertical walls

    !is_opp_player_first:
        ?determines which player moves first in the game.
        ? If isHumanPlayerFirst is true, the human player’s pawn is the light-colored one and moves first.
        ? If isHumanPlayerFirst is false, the computer’s pawn is the light-colored one and moves first.
    */
public:
    // default constructor
    GameBoard() = default;
    // Parameterized constructor
    GameBoard(bool is_opp_player_first, bool flag = false);
    // Destructor
    ~GameBoard() = default;
    // Copy Constructor
    GameBoard(const GameBoard &other);
    // Copy Assignment Operator
    GameBoard &operator=(const GameBoard &other);
    // Move Constructor
    GameBoard(GameBoard &&other) noexcept;
    // Move Assignment Operator
    GameBoard &operator=(GameBoard &&other) noexcept;

    // Getter for pawns
    const std::vector<Player> &get_pawns() const;
    // Getter for walls
    std::pair<const std::vector<std::vector<bool>> &, const std::vector<std::vector<bool>> &> get_walls() const;

    //setter for pawns
    std::vector<std::vector<bool>> set_horizontal_wall();
    std::vector<std::vector<bool>> set_vertical_wall();

    static GameBoard game_board_clone(const GameBoard& game_board);
private:
    std::vector<Player> pawns;
    struct Walls
    {
        std::vector<std::vector<bool>> horizontal;
        std::vector<std::vector<bool>> vertical;
    } walls;
};

#endif