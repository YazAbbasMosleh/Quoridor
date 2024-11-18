#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <vector>
#include <utility>
#include "utility.cpp"
#include <map>
#include <memory>

class Position
{
    /*
    !row:
        ?number of the rows in the game board
    !col:
        ?number of columns in the game board
    */

public:
    Position() = default;                              // default constructor
    Position(int row, int col) : row(row), col(col) {} // constructor
    ~Position() = default;                             // destructor
    Position(const Position &other);                   // copy constructor
    Position &operator=(const Position &other);        // copy assignment operator
    Position(Position &&other) noexcept;               // move constructor
    Position &operator=(Position &&other) noexcept;    // move assignment operator

    bool is_equal(const Position &other_position) const;

    // performig the new position after movement
    Position new_position_after_move(const std::pair<int, int> &pawn_move_pair) const;

    // gettters
    int get_row() const;
    int get_col() const;

    // setters
    void set_row(int &value);
    void set_col(int &value);

    // operators
    bool operator==(const Position &other) const
    {
        return (row == other.row && col == other.col);
    }

    bool operator!=(const Position &other) const
    {
        return !(*this == other);
    }

    static Position position_clone(const Position &position);
    std::pair<int, int> displacement_from(const Position &position) const;

private:
    int row;
    int col;
};

#endif