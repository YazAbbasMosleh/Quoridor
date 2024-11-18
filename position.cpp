#include <iostream>
#include <vector>
#include <utility>
#include "utility.cpp"
#include "position.h"

Position::Position(int row, int col) : row(row), col(col) {} // constructor implemetation

Position::Position(const Position &other) : row(other.row), col(other.col) {} // copy constructor implementation

// Copy Assignment Operator
Position &Position::operator=(const Position &other)
{
    if (this != &other)
    {
        row = other.row;
        col = other.col;
    }
    return *this;
}

// Move Constructor
Position::Position(Position &&other) noexcept : row(std::move(other.row)), col(std::move(other.col))
{
    // Reset the moved object
    other.row = 0;
    other.col = 0;
}

// Move Assignment Operator
Position &Position::operator=(Position &&other) noexcept
{
    if (this != &other)
    {
        row = std::move(other.row);
        col = std::move(other.col);

        // Reset the moved object
        other.row = 0;
        other.col = 0;
    }
    return *this;
}

// Move Assignment Operator
Position &Position::operator=(Position &&other) noexcept
{
    if (this != &other)
    {
        row = std::move(other.row);
        col = std::move(other.col);

        // Reset the moved object
        other.row = 0;
        other.col = 0;
    }
    return *this;
}

bool Position::is_equal(const Position &other_position) const
{
    return this->row == other_position.row && this->col == other_position.col;
}

// performig the new position after movement
Position Position::new_position_after_move(const std::pair<int, int> &pawn_move_pair) const
{
    return Position(this->row + pawn_move_pair.first, this->col + pawn_move_pair.second);
}

// getters
int Position::get_row() const { return row; }
int Position::get_col() const { return col; }

// setters
void Position::set_row(int &value) { row = value; }
void Position::set_col(int &value) { col = value; }

Position Position::position_clone(const Position &position)
{
    return Position(position.row, position.col);
}

std::pair<int, int> Position::displacement_from(const Position &position) const
{
    return {this->row - position.row, this->col - position.col};
}
