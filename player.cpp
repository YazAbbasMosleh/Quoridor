#include "player.h"

// constructor implementation
Player::Player(int pawn_index, bool is_opp_side, bool is_opp_player, bool flag)
{
    if (!flag)
    {
        this->pawn_index = pawn_index;
        this->is_opp_side = is_opp_side;
        this->is_opp_player = is_opp_player;
        if (is_opp_side)
        {
            this->is_opp_side = true;
            this->position = Position(8, 4);
            this->goal_row = 0;
        }
        else
        {
            this->is_opp_side = false;
            this->position = Position(0, 4);
            this->goal_row = 8;
        }
        this->num_left_walls = 10;
    }
}

// copy constructor implementation
Player::Player(const Player &other)
    : pawn_index(other.pawn_index), is_opp_side(other.is_opp_side), is_opp_player(other.is_opp_player),
      position(other.position), goal_row(other.goal_row), num_left_walls(other.num_left_walls) {}

// copy assignment operator implementation
Player &Player::operator=(const Player &other)
{
    if (this != &other)
    {
        pawn_index = other.pawn_index;
        is_opp_side = other.is_opp_side;
        is_opp_player = other.is_opp_player;
        position = other.position;
        goal_row = other.goal_row;
        num_left_walls = other.num_left_walls;
    }
    return *this;
}

// move constructor implementation
Player::Player(Player &&other) noexcept
    : pawn_index(std::move(other.pawn_index)), is_opp_side(std::move(other.is_opp_side)), is_opp_player(std::move(other.is_opp_player)),
      position(std::move(other.position)), goal_row(std::move(other.goal_row)), num_left_walls(std::move(other.num_left_walls))
{
    other.pawn_index = 0;
    other.is_opp_side = false;
    other.is_opp_player = false;
    other.position = Position();
    other.goal_row = 0;
    other.num_left_walls = 0;
}

// move assignment operator implementation
Player &Player::operator=(Player &&other) noexcept
{
    if (this != &other)
    {
        pawn_index = std::move(other.pawn_index);
        is_opp_side = std::move(other.is_opp_side);
        is_opp_player = std::move(other.is_opp_player);
        position = std::move(other.position);
        goal_row = std::move(other.goal_row);
        num_left_walls = std::move(other.num_left_walls);

        other.pawn_index = 0;
        other.is_opp_side = false;
        other.is_opp_player = false;
        other.position = Position();
        other.goal_row = 0;
        other.num_left_walls = 0;
    }
    return *this;
}

// getters implementation
int Player::get_pawn_index() const { return pawn_index; }
bool Player::get_is_opp_player() const { return is_opp_player; }
bool Player::get_is_opp_side() const { return is_opp_side; }
Position Player::get_position() const { return position; }
int Player::get_goal_row() const { return goal_row; }
int Player::get_num_left_walls() const { return num_left_walls; }

// setters implementation
void Player::set_pawn_index(int value) { this->pawn_index = value; }
void Player::set_is_opp_player(bool value) { this->is_opp_player = value; }
void Player::set_is_opp_side(bool value) { this->is_opp_side = value; }
void Player::set_position(Position value) { this->position = value; }
void Player::set_goal_row(int value) { this->goal_row = value; }
void Player::set_num_left_walls(int value) { this->num_left_walls = value; }

Player Player::player_clone(const Player &player)
{
    return Player(player.pawn_index, player.is_opp_side, player.is_opp_player, false);
}