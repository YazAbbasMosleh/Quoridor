#include "gamerule.h"
#include "utility.cpp"

class Player;
class Position;
class GameBoard;

// default constructor
#include "gamerule.h"

// default constructor
GameRule::GameRule() : board{nullptr}, winner{nullptr}, turn{0},
                       valid_next_walls({array_initialization(8, 8, true),
                                         array_initialization(8, 8, true)}),
                       probable_next_walls({array_initialization(8, 8, false),
                                            array_initialization(8, 8, false)}),
                       probable_valid_next_walls({array_initialization(8, 8, false),
                                                  array_initialization(8, 8, false)}),
                       open_ways({array_initialization(8, 9, true),
                                  array_initialization(9, 8, true)}),
                       valid_next_positions(array_initialization(9, 9, true)),
                       is_updated_probable_valid_next_walls(false),
                       is_updated_valid_next_position(false) {}
// constructor
GameRule::GameRule(bool is_opp_first, bool flag = false)
{
    if (!flag)
    {
        board = std::make_unique<GameBoard>(is_opp_first);
        winner = nullptr;
        turn = 0;
        valid_next_walls = {array_initialization(8, 8, true),
                            array_initialization(8, 8, true)};
        probable_next_walls = {array_initialization(8, 8, false),
                               array_initialization(8, 8, false)};
        open_ways = {array_initialization(8, 9, true),
                     array_initialization(9, 8, true)};
        valid_next_positions = array_initialization(9, 9, false);
        is_updated_probable_valid_next_walls = false;
        is_updated_valid_next_position = false;
    }
}

// copy constructor
GameRule::GameRule(const GameRule &other) : board{std::make_unique<GameBoard>(*other.board)},
                                            winner{other.winner}, turn{other.turn}, valid_next_walls{other.valid_next_walls},
                                            probable_next_walls{other.probable_next_walls},
                                            open_ways{other.open_ways}, valid_next_positions{other.valid_next_positions},
                                            is_updated_probable_valid_next_walls{other.is_updated_probable_valid_next_walls},
                                            is_updated_valid_next_position{other.is_updated_valid_next_position} {}

// copy assignment operator
GameRule &GameRule::operator=(const GameRule &other)
{
    if (this == &other)
        return *this;
    board = std::make_unique<GameBoard>(*other.board);
    winner = other.winner;
    turn = other.turn;
    valid_next_walls = other.valid_next_walls;
    probable_next_walls = other.probable_next_walls;
    open_ways = other.open_ways;
    valid_next_positions = other.valid_next_positions;
    is_updated_probable_valid_next_walls = other.is_updated_probable_valid_next_walls;
    is_updated_valid_next_position = other.is_updated_valid_next_position;
    return *this;
}

// move constructor
GameRule::GameRule(GameRule &&other) noexcept : board{std::move(other.board)},
                                                winner{other.winner}, turn{other.turn}, valid_next_walls{std::move(other.valid_next_walls)},
                                                probable_next_walls{std::move(other.probable_next_walls)},
                                                open_ways{std::move(other.open_ways)},
                                                valid_next_positions{std::move(other.valid_next_positions)},
                                                probable_valid_next_walls{other.probable_valid_next_walls},
                                                is_updated_valid_next_position{other.is_updated_valid_next_position}
{
    other.winner = nullptr;
    other.turn = 0;
    other.is_updated_probable_valid_next_walls = false;
    other.is_updated_valid_next_position = false;
}

// move assignment operator
GameRule &GameRule::operator=(GameRule &&other) noexcept
{
    if (this == &other)
        return *this;
    board = std::move(other.board);
    winner = other.winner;
    turn = other.turn;
    valid_next_walls = std::move(other.valid_next_walls);
    probable_next_walls = std::move(other.probable_next_walls);
    open_ways = std::move(other.open_ways);
    valid_next_positions = std::move(other.valid_next_positions);
    is_updated_probable_valid_next_walls = other.is_updated_probable_valid_next_walls;
    is_updated_valid_next_position = other.is_updated_valid_next_position;

    // reseting
    other.winner = nullptr;
    other.turn = 0;
    other.is_updated_probable_valid_next_walls = false;
    other.is_updated_valid_next_position = false;
    return *this;
}

// getters
int GameRule::get_turn() const { return turn; }
const Player *GameRule::get_player_of_index_zero() const { return &(board->get_pawns()[0]); }
const Player *GameRule::get_player_of_index_one() const { return &(board->get_pawns()[1]); }
int GameRule::get_pawn_index_of_turn() const { return turn % 2; }
int GameRule::get_pawn_index_of_not_turn() const { return (turn + 1) % 2; }
const Player *GameRule::get_pawn_of_turn() const { return &(board->get_pawns()[get_pawn_index_of_turn()]); }
const Player *GameRule::get_pawn_of_not_turn() const { return &(board->get_pawns()[get_pawn_index_of_not_turn()]); }

std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> GameRule::get_probable_valid_next_walls()
{
    if (is_updated_probable_valid_next_walls)
    {
        return probable_valid_next_walls;
    }
    is_updated_probable_valid_next_walls = true;

    // Near already placed walls
    // walls that are already placed on the board.
    auto probable_valid_next_walls = probable_next_walls;

    // Leftmost and rightmost horizontal walls after several turns
    if (turn >= 6)
    {
        for (int i = 0; i < 8; i++)
        {
            probable_valid_next_walls.first[i][0] = true;
            probable_valid_next_walls.first[i][7] = true;
        }
    }

    // Near pawns: place walls to disturb opponent or support myself only after several turns
    if (turn >= 3)
    {
        // Disturb opponent
        set_walls(probable_valid_next_walls, get_pawn_of_not_turn());
    }
    if (turn >= 6 || index_of_value_in_2D_array(board->get_walls().first, true).size() > 0 || index_of_value_in_2D_array(board->get_walls().second, true).size() > 0)
    {
        // Support myself
        set_walls(probable_valid_next_walls, get_pawn_of_turn());
    }
    probable_valid_next_walls.first = array_2D_and_operation(probable_valid_next_walls.first, valid_next_walls.first);
    probable_valid_next_walls.second = array_2D_and_operation(probable_valid_next_walls.second, valid_next_walls.second);

    return probable_valid_next_walls;
}

std::vector<std::vector<bool>> GameRule::get_next_positions()
{
    if (is_updated_valid_next_position)
    {
        return valid_next_positions;
    }
    is_updated_valid_next_position = true;

    valuing_2D_array(valid_next_positions, false);

    set_valid_move(MOVE_UP, MOVE_LEFT, MOVE_RIGHT);
    set_valid_move(MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT);
    set_valid_move(MOVE_LEFT, MOVE_UP, MOVE_DOWN);
    set_valid_move(MOVE_RIGHT, MOVE_UP, MOVE_DOWN);

    return valid_next_positions;
}

Player *GameRule::get_winner() const
{
    return winner;
}

// setter
void GameRule::set_turn(int value)
{
    turn = value;
    is_updated_valid_next_position = false;
    is_updated_probable_valid_next_walls = false;
}

/*
!"set_valid_move" function
    ? function is designed to calculate and set the valid next positions
    ? for the pawn of the current turn in a specific direction on the GameRule::gameRule board
        *Get the current position
        *Check if the main move is valid
        *If the main move is valid
        *If the other pawn is at the main move position
            !Check for jumping in the main direction
            !If the pawn cannot jump in the main direction
        *If the other pawn is not at the main move position

*/

void GameRule::set_valid_move(const std::vector<int> &main_move, const std::vector<int> sub_move1, const std::vector<int> sub_move2)
{
    Position current_position = get_pawn_of_turn()->get_position();

    std::pair<int, int> move_pair(main_move[0], main_move[1]);
    if (is_valid_next_move_not_considering_other_pawn(current_position, main_move))
    {
        Position position_of_pawn_after_1main_move = current_position.new_position_after_move(move_pair);
        if (position_of_pawn_after_1main_move.is_equal(get_pawn_of_not_turn()->get_position()))
        {
            if (is_valid_next_move_not_considering_other_pawn(position_of_pawn_after_1main_move, main_move))
            {
                Position position_of_pawn_after_2main_move = position_of_pawn_after_1main_move.new_position_after_move(move_pair);
                valid_next_positions[position_of_pawn_after_2main_move.get_row()][position_of_pawn_after_2main_move.get_col()] = true;
            }
            else
            {
                std::pair<int, int> sub_move_1_pair(sub_move1[0], sub_move1[1]);

                if (is_valid_next_move_not_considering_other_pawn(position_of_pawn_after_1main_move, sub_move1))
                {
                    Position position_of_move_after_main_and_sub1_move = position_of_pawn_after_1main_move.new_position_after_move(sub_move_1_pair);
                    valid_next_positions[position_of_move_after_main_and_sub1_move.get_row()][position_of_move_after_main_and_sub1_move.get_col()] = true;
                }

                std::pair<int, int> sub_move_2_pair(sub_move2[0], sub_move2[1]);

                if (is_valid_next_move_not_considering_other_pawn(position_of_pawn_after_1main_move, sub_move2))
                {
                    Position position_of_move_after_main_and_sub2_move = position_of_pawn_after_1main_move.new_position_after_move(sub_move_2_pair);
                    valid_next_positions[position_of_move_after_main_and_sub2_move.get_row()][position_of_move_after_main_and_sub2_move.get_col()] = true;
                }
            }
        }
        else
        {
            valid_next_positions[position_of_pawn_after_1main_move.get_row()][position_of_pawn_after_1main_move.get_col()] = true;
        }
    }
}

// methods
/*
!"is_valid_next_move_not_considering_other_pawn" function
    ? checks if a move from the current position in a specified
    ? direction is valid, without considering the position of the
    ? other pawn.
        *Check the direction of the move
        *Check if the move is within the board boundaries
        *Check if the move is not blocked by a wall
        *Return the result: The function returns true if the move is within the board boundaries and not blocked by a wall, and false otherwise.
*/

bool GameRule::is_valid_next_move_not_considering_other_pawn(const Position &current_position, const std::vector<int> &pawn_move)
{
    if (pawn_move == MOVE_UP)
    {
        return (current_position.get_row() > 0 &&
                open_ways.first[current_position.get_row() - 1][current_position.get_col()]);
    }
    if (pawn_move == MOVE_DOWN)
    {
        return (current_position.get_row() < 8 &&
                open_ways.first[current_position.get_row()][current_position.get_col()]);
    }
    else if (pawn_move == MOVE_LEFT)
    {
        return (current_position.get_col() > 0 &&
                open_ways.second[current_position.get_row()][current_position.get_col() - 1]);
    }
    else if (pawn_move == MOVE_RIGHT)
    {
        return (current_position.get_col() < 8 &&
                open_ways.second[current_position.get_row()][current_position.get_col()]);
    }
    else
    {
        throw std::invalid_argument("pawn move tuple should be one MOVE_UP , MOVE_DOWN , MOVE_LEWFT , MOVE_RIGHT");
    }
}

/*
!"move_pawn" function:
?to move a pawn in the GameRule::gameRule to a specified row and column
? on the GameRule::gameRule board.
? If the move is valid or if flag is false,
? the function updates the position of the pawn of the current turn to the specified row and column.
*/

bool GameRule::move_pawn(int row, int col, bool flag = false)
{
    if (flag && !valid_next_positions[row][col])
    {
        return false;
    }

    Player *pawn_of_turn = const_cast<Player *>(get_pawn_of_turn());
    pawn_of_turn->get_position().set_row(row);
    pawn_of_turn->get_position().set_col(col);
    if (pawn_of_turn->get_goal_row() == pawn_of_turn->get_position().get_row())
    {
        winner = pawn_of_turn;
    }
    turn++;
    return true;
}

/*
! "is_there_open_way" fucntion:
    ?check if a pawn can move in a certain direction from
    ? a given position on the GameRule::gameRule board
        *current_row
        *current_col
        *pawn_move :represents the move direction for the pawn:
            *MOVE_UP
            *MOVE_DOWN
            *MOVE_LEFT
            *MOVE_RIGHT
*/
bool GameRule::is_there_open_way(int current_row, int current_col, const std::vector<int> &pawn_move)
{
    if (pawn_move == MOVE_UP)
    {
        return (current_row > 0 && open_ways.first[current_row - 1][current_col]);
    }
    else if (pawn_move == MOVE_DOWN)
    {
        return (current_row < 8 && open_ways.first[current_row][current_col]);
    }
    else if (pawn_move == MOVE_LEFT)
    {
        return (current_col > 0 && open_ways.second[current_row][current_col - 1]);
    }
    else if (pawn_move == MOVE_RIGHT)
    {
        return (current_col < 8 && open_ways.second[current_row][current_col]);
    }
    else
    {
        throw std::invalid_argument("pawn move tuple should be one of MOVE_UP , MOVE_DOWN , MOVE_LEFT , MOVE_RIGHT");
    }
}

/*
!"test_if_adjecent_to_other_wall_for_horizontal_wall_left" function
    ? This function checks if there is a wall adjacent to the
    ? left side of a potential horizontal wall placement on the GameRule::gameRule board.
        *Check if the column is valid
        *Check for vertical walls to the left
        *Check for vertical walls diagonally up-left and down-left
        *Check for horizontal walls further to the left
*/

bool GameRule::is_adjacent_left_wall_horizontal(int row, int col)
{
    if (col >= 1)
    {
        auto walls = board->get_walls();
        if (walls.second[row][col - 1])
        {
            return true;
        }
        if (row >= 1)
        {
            if (walls.second[row - 1][col - 1])
            {
                return true;
            }
        }
        if (row <= 6)
        {
            if (walls.second[row + 1][col - 1])
            {
                return true;
            }
        }
        if (col >= 2)
        {
            if (walls.first[row][col - 2])
            {
                return true;
            }
        }
    }
    return false;
}

bool GameRule::is_adjacent_right_wall_horizontal(int row, int col)
{
    if (col <= 6)
    {
        if (board->get_walls().second[row][col + 1])
        {
            return true;
        }
        if (row >= 1)
        {
            if (board->get_walls().second[row - 1][col + 1])
            {
                return true;
            }
        }
        if (row <= 6)
        {
            if (board->get_walls().second[row + 1][col + 1])
            {
                return true;
            }
        }
        if (col <= 5)
        {
            if (board->get_walls().first[row][col + 2])
            {
                return true;
            }
        }
    }
    return false;
}

/*
! check whether there is a wall adjacent to the middle (vertical)
! side of a potential horizontal wall placement on the game board.

*/
bool GameRule::is_adjacent_middle_wall_horizontal(int row, int col)
{
    if (row >= 1)
    {
        if (board->get_walls().second[row - 1][col])
        {
            return true;
        }
    }
    if (row <= 6)
    {
        if (board->get_walls().second[row + 1][col])
        {
            return true;
        }
    }
    return false;
}

bool GameRule::is_horizontal_wall_connected(int row, int col)
{
    bool left = (col == 0 || is_adjacent_left_wall_horizontal(row, col));
    bool right = (col == 7 || is_adjacent_right_wall_horizontal(row, col));
    bool middle = is_adjacent_middle_wall_horizontal(row, col);
    return (left && right) || (right || middle) || (middle || left);
}

bool GameRule::is_adjacent_top_wall_vertical(int row, int col)
{
    if (row >= 1)
    {
        if (board->get_walls().first[row - 1][col])
        {
            return true;
        }
        if (col >= 1)
        {
            if (board->get_walls().first[row - 1][col - 1])
            {
                return true;
            }
        }
        if (col <= 6)
        {
            if (board->get_walls().first[row - 1][col + 1])
            {
                return true;
            }
        }
        if (row >= 2)
        {
            if (board->get_walls().second[row - 2][col])
            {
                return true;
            }
        }
    }
    return false;
}

bool GameRule::is_adjacent_bottom_wall_vertical(int row, int col)
{
    if (row <= 6)
    {
        if (board->get_walls().first[row + 1][col])
        {
            return true;
        }
        if (col >= 1)
        {
            if (board->get_walls().first[row + 1][col - 1])
            {
                return true;
            }
        }
        if (col <= 6)
        {
            if (board->get_walls().first[row + 1][col + 1])
            {
                return true;
            }
        }
        if (row <= 5)
        {
            if (board->get_walls().second[row + 2][col])
            {
                return true;
            }
        }
    }
    return false;
}

bool GameRule::is_adjacent_middle_wall_vertical(int row, int col)
{
    if (col >= 1)
    {
        if (board->get_walls().first[row][col - 1])
        {
            return true;
        }
    }
    if (col <= 6)
    {
        if (board->get_walls().first[row][col + 1])
        {
            return true;
        }
    }
    return false;
}

bool GameRule::is_vertical_wall_connected(int row, int col)
{
    bool top = (row == 0 || is_adjacent_top_wall_vertical(row, col));
    bool bottom = (row == 7 || is_adjacent_bottom_wall_vertical(row, col));
    bool middle = is_adjacent_middle_wall_vertical(row, col);

    return (top && bottom) || (bottom && middle) || (middle && top);
}

/*
!"can_place_vertical_wall_safely" function:
?checks whether placing a vertical wall at the specified row and column
? would still allow both pawns to have a path to their respective goal lines
    *Check if the wall is connected on two points
    *Temporarily place the wal
    *Check if paths to goal lines exist
    *Remove the temporary wall
    *Return the result
*/

bool GameRule::can_place_vertical_wall_safely(int row, int col)
{
    if (!is_vertical_wall_connected(row, col))
    {
        return true;
    }
    open_ways.second[row][col] = false;
    open_ways.second[row + 1][col] = false;
    bool result = is_path_to_goal_for_both();
    open_ways.second[row][col] = true;
    open_ways.second[row + 1][col] = true;
    return result;
}
/*
!"can_place_horizontal_wall_safely" function:
?checks whether placing a horizontal wall at the specified row and column
? would still allow both pawns to have a path to their respective goal lines
    *Check if the wall is connected on two points
    *Temporarily place the wal
    *Check if paths to goal lines exist
    *Remove the temporary wall
    *Return the result
*/
bool GameRule::can_place_horizontal_wall_safely(int row, int col)
{
    if (!is_horizontal_wall_connected(row, col))
    {
        return true;
    }
    open_ways.first[row][col] = false;
    open_ways.first[row][col + 1] = false;
    bool result = is_path_to_goal_for_both();
    open_ways.first[row][col] = true;
    open_ways.first[row][col + 1] = true;
    return result;
}

/*
!"is_valid_move" function:
    ?determines if a given move (either moving a pawn or placing a wall)
    ? is valid according to the GameRule::gameRule's rules.
    ? The move is represented as a vector of vectors, where each inner vector represents a different type of move
*/
bool GameRule::is_valid_move(std::vector<std::vector<int>> move)
{
    std::vector<int> move_pawn_to = move[0];
    std::vector<int> place_horizontal_wall_at = move[1];
    std::vector<int> place_vertical_wall_at = move[2];

    if (!move_pawn_to.empty())
    {
        return this->valid_next_positions[move_pawn_to[0]][move_pawn_to[1]];
    }
    else if (!place_horizontal_wall_at.empty())
    {
        return this->can_place_horizontal_wall_safely(place_horizontal_wall_at[0], place_horizontal_wall_at[1]);
    }
    else if (!place_vertical_wall_at.empty())
    {
        return this->can_place_vertical_wall_safely(place_vertical_wall_at[0], place_vertical_wall_at[1]);
    }
    return false;
}
/*
!"update_probable_valid_next_wall_after_placing_vertical_wall" function:
    ?update the _probable_valid_next_walls data structure
    ? after placing a horizontal wall on the GameRule::gameRule board
        *Check if the wall is at the top edge of the board
        *Check if the wall is at the bottom edge of the board
        *Check if the wall is at the left edge of the board
        *Check if the wall is at the right edge of the board
*/

void GameRule::update_probable_next_walls_after_placing_vertical_wall(int row, int col)
{
    if (col >= 1)
    {
        this->probable_next_walls.second[row][col - 1] = true;
    }
    if (col <= 6)
    {
        this->probable_next_walls.second[row][col + 1] = true;
    }
    if (row >= 1)
    {
        this->probable_next_walls.second[row - 1][col] = true;
        if (col >= 1)
        {
            this->probable_next_walls.second[row - 1][col - 1] = true;
        }
        if (col <= 6)
        {
            this->probable_next_walls.second[row - 1][col + 1] = true;
        }
        if (row >= 2)
        {
            this->probable_next_walls.first[row - 2][col] = true;
            this->probable_next_walls.second[row - 2][col] = true;
            if (row >= 3)
            {
                this->probable_next_walls.first[row - 3][col] = true;
            }
        }
    }
    if (row <= 6)
    {
        this->probable_next_walls.second[row + 1][col] = true;
        if (col >= 1)
        {
            this->probable_next_walls.second[row + 1][col - 1] = true;
        }
        if (col <= 6)
        {
            this->probable_next_walls.second[row + 1][col + 1] = true;
        }
        if (row <= 5)
        {
            this->probable_next_walls.first[row + 2][col] = true;
            this->probable_next_walls.second[row + 2][col] = true;
            if (row <= 4)
            {
                this->probable_next_walls.first[row + 3][col] = true;
            }
        }
    }
}

void GameRule::update_probable_valid_next_wall_after_placing_horizontal_wall(int row, int col)
{
    if (row >= 1)
    {
        this->probable_next_walls.first[row - 1][col] = true;
    }
    if (row <= 6)
    {
        this->probable_next_walls.first[row + 1][col] = true;
    }
    if (col >= 1)
    {
        this->probable_next_walls.first[row][col - 1] = true;
        if (row >= 1)
        {
            this->probable_next_walls.first[row - 1][col - 1] = true;
        }
        if (row <= 6)
        {
            this->probable_next_walls.first[row + 1][col - 1] = true;
        }
        if (col >= 2)
        {
            this->probable_next_walls.second[row][col - 2] = true;
            this->probable_next_walls.first[row][col - 2] = true;
            if (col >= 3)
            {
                this->probable_next_walls.second[row][col - 3] = true;
            }
        }
    }
    if (col <= 6)
    {
        this->probable_next_walls.first[row][col + 1] = true;
        if (row >= 1)
        {
            this->probable_next_walls.first[row - 1][col + 1] = true;
        }
        if (row <= 6)
        {
            this->probable_next_walls.first[row + 1][col + 1] = true;
        }
        if (col <= 5)
        {
            this->probable_next_walls.second[row][col + 2] = true;
            this->probable_next_walls.first[row][col + 2] = true;
            if (col <= 4)
            {
                this->probable_next_walls.second[row][col + 3] = true;
            }
        }
    }
}

/*
!"is_path_to_goal_for_both" function
    ?Check if there exist paths to the goal lines for both pawns
*/

bool GameRule::is_path_to_goal_for_both()
{
    const Player *pawn1 = get_pawn_of_turn();
    const Player *pawn2 = get_pawn_of_not_turn();
    return (is_path_to_goal(*pawn1) && is_path_to_goal(*pawn2));
}

bool GameRule::place_horizontal_wall(int row, int col, bool flag = false)
{
    if (flag && !this->can_place_horizontal_wall_safely(row, col))
    {
        return false;
    }
    this->open_ways.first[row][col] = false;
    this->open_ways.first[row][col + 1] = false;
    this->valid_next_walls.second[row][col] = false;
    this->valid_next_walls.first[row][col] = false;
    if (col > 0)
    {
        this->valid_next_walls.first[row][col - 1] = false;
    }
    if (col < 7)
    {
        this->valid_next_walls.second[row][col + 1] = false;
    }
    this->board->set_horizontal_wall()[row][col] = true;

    this->update_probable_valid_next_wall_after_placing_horizontal_wall(row, col);
    // Pawn *pawn_of_turn = const_cast<Pawn *>(get_pawn_of_turn());
    int val = this->get_pawn_of_turn()->get_num_left_walls();
    const_cast<Player *>(this->get_pawn_of_turn())->set_num_left_walls(val - 1);
    this->turn++;
    return true;
}

bool GameRule::place_vertical_wall(int row, int col, bool flag = false)
{
    if (flag && !this->can_place_vertical_wall_safely(row, col))
    {
        return false;
    }
    this->open_ways.second[row][col] = false;
    this->open_ways.second[row + 1][col] = false;
    this->valid_next_walls.first[row][col] = false;
    this->valid_next_walls.second[row][col] = false;
    if (row > 0)
    {
        this->valid_next_walls.second[row - 1][col] = false;
    }
    if (row < 7)
    {
        this->valid_next_walls.second[row + 1][col] = false;
    }
    this->board->set_vertical_wall()[row][col] = true;

    this->update_probable_next_walls_after_placing_vertical_wall(row, col);
    // Pawn *pawn_of_turn = const_cast<Pawn *>(get_pawn_of_turn());
    int val = this->get_pawn_of_turn()->get_num_left_walls();
    const_cast<Player *>(this->get_pawn_of_turn())->set_num_left_walls(val - 1);
    this->turn++;
    return true;
}

bool GameRule::do_move(const std::vector<std::vector<int>> &move, bool flag = false)
{
    if (this->winner != nullptr)
    {
        std::cout << "error: doMove after already terminal......" << std::endl; // for debug
    }
    const std::vector<int> &position = move[0];
    const std::vector<int> &position_of_horizontal_wall_placing = move[1];
    const std::vector<int> &position_of_vertical_wall_placing = move[2];
    if (!position.empty())
    {
        return this->move_pawn(position[0], position[1], flag);
    }
    else if (!position_of_horizontal_wall_placing.empty())
    {
        return this->place_horizontal_wall(position_of_horizontal_wall_placing[0], position_of_horizontal_wall_placing[1], flag);
    }
    else if (!position_of_vertical_wall_placing.empty())
    {
        return this->place_vertical_wall(position_of_vertical_wall_placing[0], position_of_vertical_wall_placing[1], flag);
    }
    return false;
}

GameRule GameRule::game_rule_clone(const GameRule &game_rule)
{
    GameRule cloned_game_rule(true, true);

    // Clone the game board
    // Clone the game board
    cloned_game_rule.board = std::make_unique<GameBoard>(*game_rule.board);

    // Clone the winner
    // Clone the winner
    if (game_rule.winner == nullptr)
    {
        cloned_game_rule.winner = nullptr;
    }
    else
    {
        cloned_game_rule.winner = std::make_unique<Player>(*game_rule.winner).release();
    }

    cloned_game_rule.turn = game_rule.turn;

    cloned_game_rule.valid_next_walls.first = cloned_array(game_rule.valid_next_walls.first);
    cloned_game_rule.valid_next_walls.second = cloned_array(game_rule.valid_next_walls.second);

    cloned_game_rule.probable_next_walls.first = cloned_array(game_rule.probable_next_walls.first);
    cloned_game_rule.probable_next_walls.second = cloned_array(game_rule.probable_next_walls.second);

    cloned_game_rule.probable_valid_next_walls = std::make_pair(std::vector<std::vector<bool>>{}, std::vector<std::vector<bool>>{});
    cloned_game_rule.is_updated_probable_valid_next_walls = false;

    cloned_game_rule.open_ways.first = cloned_array(game_rule.open_ways.first);
    cloned_game_rule.open_ways.second = cloned_array(game_rule.open_ways.second);

    cloned_game_rule.valid_next_positions = cloned_array(game_rule.valid_next_positions);
    cloned_game_rule.is_updated_valid_next_position = game_rule.is_updated_valid_next_position;

    return cloned_game_rule;
}

std::vector<std::pair<int, int>> GameRule::get_valid_position() const
{
    return index_of_value_in_2D_array(this->valid_next_positions, true);
}

std::vector<std::pair<int, int>> GameRule::get_valid_horizontal_wall()
{
    std::vector<std::pair<int, int>> next_horizontal_walls = index_of_value_in_2D_array(valid_next_walls.first, true);
    std::vector<std::pair<int, int>> no_block_next_horizontal_walls;

    for (const auto &pos : next_horizontal_walls)
    {
        if (can_place_horizontal_wall_safely(pos.first, pos.second))
        {
            no_block_next_horizontal_walls.emplace_back(pos);
        }
    }
    return no_block_next_horizontal_walls;
}

std::vector<std::pair<int, int>> GameRule::get_valid_vertical_wall()
{
    std::vector<std::pair<int, int>> next_vertical_walls = index_of_value_in_2D_array(valid_next_walls.second, true);
    std::vector<std::pair<int, int>> no_block_next_verticals;

    for (const auto &pos : next_vertical_walls)
    {
        if (can_place_vertical_wall_safely(pos.first, pos.second))
        {
            no_block_next_verticals.emplace_back(pos);
        }
    }
    return no_block_next_verticals;
}

std::vector<std::pair<int, int>> GameRule::get_safe_probable_horizontal_walls()
{
    std::vector<std::pair<int, int>> next_horizontal_walls = index_of_value_in_2D_array(probable_valid_next_walls.first, true);
    std::vector<std::pair<int, int>> safe_next_horizontal_walls;

    for (const auto &pos : next_horizontal_walls)
    {
        if (can_place_horizontal_wall_safely(pos.first, pos.second))
        {
            safe_next_horizontal_walls.emplace_back(pos);
        }
    }
    return safe_next_horizontal_walls;
}

std::vector<std::pair<int, int>> GameRule::get_safe_probable_vertical_walls()
{
    std::vector<std::pair<int, int>> next_vertical_walls = index_of_value_in_2D_array(probable_valid_next_walls.second, true);
    std::vector<std::pair<int, int>> safe_next_vertical_walls;

    for (const auto &pos : next_vertical_walls)
    {
        if (can_place_vertical_wall_safely(pos.first, pos.second))
        {
            safe_next_vertical_walls.emplace_back(pos);
        }
    }
    return safe_next_vertical_walls;
}

// std::vector<std::pair<int , int>> GameRule::get_safe_next_position(const Player &pawn) const
// {
//     auto
// }