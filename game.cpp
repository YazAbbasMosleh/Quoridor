#include <vector>
#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <functional>

class Pawn;
class Pawnposition;
class Board;
class Game;

// pawn movement representation
const std::vector<int> MOVE_UP = {-1, 0};
const std::vector<int> MOVE_DOWN = {1, 0};
const std::vector<int> MOVE_RIGHT = {0, 1};
const std::vector<int> MOVE_LEFT = {0, -1};

// Function for initialization of the board game
std::vector<std::vector<bool>> create2DArrayInitializedTo(int num_row, int num_col, bool initial_value)
{
    return std::vector<std::vector<bool>>(num_row, std::vector<bool>(num_col, initial_value));
}

// setting every element of the game board to a value
void set2DArrayEveryElementToValue(std::vector<std::vector<bool>> &arr2D_original, bool value)
{
    for (std::size_t i{}; i < arr2D_original.size(); ++i)
    {
        for (std::size_t j{}; j < arr2D_original[0].size(); ++j)
        {
            arr2D_original[i][j] = value;
        }
    }
}

// creation of the copy of the 2D-array
std::vector<std::vector<int>> createCopyOfGameBoard(const std::vector<std::vector<int>> &arr2D_original)
{
    std::vector<std::vector<int>> arr2D_copy = arr2D_original;
    return arr2D_copy;
}

// logical operation between 2 states
std::vector<std::vector<bool>> logicalAndBetween2DArray(const std::vector<std::vector<bool>> &arr2D_original_A, const std::vector<std::vector<bool>> &arr2D_original_B)
{
    std::vector<std::vector<bool>> arr2D_original_bool(arr2D_original_A.size(), std::vector<bool>(arr2D_original_A[0].size()));
    for (std::size_t i{}; i < arr2D_original_A.size(); ++i)
    {
        for (std::size_t j{}; j < arr2D_original_A[0].size(); ++j)
        {
            arr2D_original_bool[i][j] = arr2D_original_A[i][j] && arr2D_original_B[i][j];
        }
    }
    return arr2D_original_bool;
}

std::vector<std::pair<int, int>> indices_of_value_in_2D_array(const std::vector<std::vector<bool>> &arr2D, bool value)
{
    std::vector<std::pair<int, int>> indices;
    for (int i = 0; i < arr2D.size(); ++i)
    {
        for (int j = 0; j < arr2D[i].size(); ++j)
        {
            if (arr2D[i][j] == value)
            {
                indices.push_back({i, j});
            }
        }
    }
    return indices;
}

/*

! PawnPosition Class:

*/

class PawnPosition
{
    /*
    !row:
        ?number of the rows in the game board
    !col:
        ?number of columns in the game board
    */

public:
    PawnPosition() = default;                              // default constructor
    PawnPosition(int row, int col) : row(row), col(col) {} // constructor

    bool is_equals(const PawnPosition &other_position) const
    {
        return this->row == other_position.row && this->col ==
                                                      other_position.col;
    }

    // performig the new position after movement
    PawnPosition new_add_move(const std::pair<int, int> &pawn_move_pair) const
    {
        return PawnPosition(this->row + pawn_move_pair.first, this->col + pawn_move_pair.second);
    }

    // gettters
    int get_row() const { return row; }
    int get_col() const { return col; }

    // setters
    void set_row(int &value) { row = value; }
    void set_col(int &value) { col = value; }

private:
    int row;
    int col;
};

/*

! Pawn Class

*/

class Pawn
{
    /*
    !index:
        ?which pawn we are dealing with(0 --> first one , 1--> second one)
    !is_opp_side:
        ?indicates which side of the game board the pawn is on.
        ?If it is true, the pawn is on the opponent side of the board.
        ?If it is false, the pawn is on the computer side of the board.
    !is_opp_player:
        ? indicates whether the pawn is controlled by a human player.
        ? If it is true, the pawn is controlled by a opponent.
        ? If it is false, the pawn is controlled by the computer (AI).
    !position:
        ?current position of the pawn on the game board
    !goal_row:
        ?the row that the pawn is trying to reach
    !num_left_walls:
        ?the number of walls that the pawn has left to place on the board
    */
public:
    // default constructor
    Pawn() = default;

    // constructor
    Pawn(int index, bool is_opp_side, bool is_opp_player, bool for_clone = false)
    {
        if (!for_clone)
        {
            this->index = index;
            this->is_opp_side = is_opp_side;
            this->is_opp_player = is_opp_player;
            if (is_opp_side)
            {
                this->is_opp_side = true;
                this->position = PawnPosition(8, 4);
                this->goal_row = 0;
            }
            else
            {
                this->is_opp_side = false;
                this->position = PawnPosition(0, 4);
                this->goal_row = 8;
            }
            this->num_left_walls = 10;
        }
    }

    // getters
    int get_index() const { return index; }
    bool get_is_opp_player() const { return is_opp_player; }
    bool get_is_opp_side() const { return is_opp_side; }
    PawnPosition get_position() const { return position; }
    int get_goal_row() const { return goal_row; }
    int get_num_left_walls() const { return num_left_walls; }

    // setters
    void set_index(int value) { this->index = value; }
    void set_is_opp_player(bool value) { this->is_opp_player = value; }
    void set_is_opp_side(bool value) { this->is_opp_side = value; }
    void set_position(PawnPosition value) { this->position = value; }
    void set_goal_row(int value) { this->goal_row = value; }
    void set_num_left_walls(int value) { this->num_left_walls = value; }

private:
    int index;
    bool is_opp_side;
    bool is_opp_player;
    PawnPosition position;
    int goal_row;
    int num_left_walls;
};

/*

! Board Class
*/

class Board
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
    Board() = default;

    // Parameterized constructor
    Board(bool is_opp_player_first, bool for_clone = false)
    {
        if (!for_clone)
        {
            if (is_opp_player_first)
            {
                pawns.emplace_back(0, true, true);
                pawns.emplace_back(1, false, false);
            }
            else
            {
                pawns.emplace_back(0, false, false);
                pawns.emplace_back(1, true, true);
            }
            walls.horizontal = create2DArrayInitializedTo(8, 8, false);
            walls.vertical = create2DArrayInitializedTo(8, 8, false);
        }
    }

    // Getter for pawns
    const std::vector<Pawn> &get_pawns() const
    {
        return pawns;
    }

    // Getter for walls
    std::pair<const std::vector<std::vector<bool>> &, const std::vector<std::vector<bool>> &> get_walls() const
    {
        return {walls.horizontal, walls.vertical};
    }

private:
    std::vector<Pawn> pawns;
    struct Walls
    {
        std::vector<std::vector<bool>> horizontal;
        std::vector<std::vector<bool>> vertical;
    } walls;
};

/*
! Game Class(implemetation of the game and it's rules)
*/

class Game
{
    /*
    !board:
        ?game board on which the Quoridor game is played
    !winner:
        ?pawn that has won the game.
    !_turn:
        ?keeps track of the current turn in the game
            *placing wall
            *moving pawn
    !valid_next_walls:
        ?valid locations for placing walls on the game board
    !_probable_next_wall:
        ?the probable locations for placing walls on the next turn
    !_probable_valid_next_walls:
        ?probable wall locations that are also valid
        ? according to the valid_next_walls array
    !_probable_valid_next_walls-updated:
        ? indicates whether the _probableValidNextWalls array has been updated
        ? after the last turn. If it’s false, the _probable_valid_next_walls
        ? array needs to be recalculated based on the current game state.
    !open_ways:
        ?represent the open paths between adjacent squares on the game board.
        ?A value of true at a specific location in the array indicates
        ? that the path is open (not blocked by a wall).
        ? while false indicates that the path is blocked
        ? This array is updated each time a wall is placed on the board
    !_valid_next_positions:
        ?represents the valid positions for the pawn to move to on the next turn.
        ? A value of true at a specific location in the array indicates
        ? that the pawn can move there, while false indicates that the
        ? pawn cannot move there. This array is updated each time a pawn is moved or a wall is placed.
    !_valid_next_positions_udated:
        ?represents the valid positions for the pawn to move to on the next turn.
        ? A value of true at a specific location in the array indicates that
        ? the pawn can move there, while false indicates that the pawn cannot move there.
        ? This array is updated each time a pawn is moved or a wall is placed.
    */
public:
    // default constructor
    Game() : board{nullptr}, winner{nullptr}, _turn{0},
             valid_next_walls({create2DArrayInitializedTo(8, 8, true),
                               create2DArrayInitializedTo(8, 8, true)}),
             _probable_next_walls({create2DArrayInitializedTo(8, 8, false),
                                   create2DArrayInitializedTo(8, 8, false)}),
             open_ways({create2DArrayInitializedTo(8, 9, true),
                        create2DArrayInitializedTo(9, 8, true)}),
             _valid_next_positions(create2DArrayInitializedTo(9, 9, true)),
             _probable_valid_next_walls_updated(false), _valid_next_position_updated(false) {}
    // constructor
    Game(bool is_opp_player_first, bool for_clone = false)
    {
        if (!for_clone)
        {
            board = std::make_unique<Board>(is_opp_player_first);
            winner = nullptr;
            _turn = 0;
            valid_next_walls = {create2DArrayInitializedTo(8, 8, true),
                                create2DArrayInitializedTo(8, 8, true)};
            _probable_next_walls = {create2DArrayInitializedTo(8, 8, false),
                                    create2DArrayInitializedTo(8, 8, false)};
            open_ways = {create2DArrayInitializedTo(8, 9, true),
                         create2DArrayInitializedTo(9, 8, true)};
            _valid_next_positions = create2DArrayInitializedTo(9, 9, false);
            _probable_valid_next_walls_updated = false;
            _valid_next_position_updated = false;
        }
    }

    // destructor
    ~Game() = default;

    // copy constructor
    Game(const Game &other) : board{std::make_unique<Board>(*other.board)},
                              winner{other.winner}, _turn{other._turn}, valid_next_walls{other.valid_next_walls},
                              _probable_next_walls{other._probable_next_walls},
                              open_ways{other.open_ways}, _valid_next_positions{other._valid_next_positions},
                              _probable_valid_next_walls_updated{other._probable_valid_next_walls_updated},
                              _valid_next_position_updated{other._valid_next_position_updated} {}

    // copy assignment operator
    Game &operator=(const Game &other)
    {
        if (this == &other)
            return *this;
        board = std::make_unique<Board>(*other.board);
        winner = other.winner;
        _turn = other._turn;
        valid_next_walls = other.valid_next_walls;
        _probable_next_walls = other._probable_next_walls;
        open_ways = other.open_ways;
        _valid_next_positions = other._valid_next_positions;
        _probable_valid_next_walls_updated = other._probable_valid_next_walls_updated;
        _valid_next_position_updated = other._valid_next_position_updated;
        return *this;
    }

    // move constructor
    Game(Game &&other) noexcept : board{std::move(other.board)},
                                  winner{other.winner}, _turn{other._turn}, valid_next_walls{std::move(other.valid_next_walls)},
                                  _probable_next_walls{std::move(other._probable_next_walls)},
                                  open_ways{std::move(other.open_ways)},
                                  _valid_next_positions{std::move(other._valid_next_positions)},
                                  _probable_valid_next_walls{other._probable_valid_next_walls},
                                  _valid_next_position_updated{other._valid_next_position_updated}
    {
        other.winner = nullptr;
        other._turn = 0;
        other._probable_valid_next_walls_updated = false;
        other._valid_next_position_updated = false;
    }

    // move assignment operator
    Game &operator=(Game &&other) noexcept
    {
        if (this == &other)
            return *this;
        board = std::move(other.board);
        winner = other.winner;
        _turn = other._turn;
        valid_next_walls = std::move(other.valid_next_walls);
        _probable_next_walls = std::move(other._probable_next_walls);
        open_ways = std::move(other.open_ways);
        _valid_next_positions = std::move(other._valid_next_positions);
        _probable_valid_next_walls_updated = other._probable_valid_next_walls_updated;
        _valid_next_position_updated = other._valid_next_position_updated;

        // reseting
        other.winner = nullptr;
        other._turn = 0;
        other._probable_valid_next_walls_updated = false;
        other._valid_next_position_updated = false;
        return *this;
    }

    // getters
    int get_turn() const { return _turn; }
    const Pawn *get_pawn0() const { return &(board->get_pawns()[0]); }
    const Pawn *get_pawn1() const { return &(board->get_pawns()[1]); }
    int get_pawn_index_of_turn() const { return _turn % 2; }
    int get_pawn_index_of_not_turn() const { return (_turn + 1) % 2; }
    const Pawn *get_pawn_of_turn() const { return &(board->get_pawns()[get_pawn_index_of_turn()]); }
    const Pawn *get_pawn_of_not_turn() const { return &(board->get_pawns()[get_pawn_index_of_not_turn()]); }

    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> get_probable_valid_next_walls()
    {
        if (_probable_valid_next_walls_updated)
        {
            return _probable_valid_next_walls;
        }
        _probable_valid_next_walls_updated = true;

        // Near already placed walls
        auto _probable_valid_next_walls = _probable_next_walls;

        // Leftmost and rightmost horizontal walls after several turns
        if (_turn >= 6)
        {
            for (int i = 0; i < 8; i++)
            {
                _probable_valid_next_walls.first[i][0] = true;
                _probable_valid_next_walls.first[i][7] = true;
            }
        }

        // Near pawns: place walls to disturb opponent or support myself only after several turns
        if (_turn >= 3)
        {
            // Disturb opponent
            set_walls_beside_pawn(_probable_valid_next_walls, get_pawn_of_not_turn());
        }
        if (_turn >= 6 || indices_of_value_in_2D_array(board->get_walls().first, true).size() > 0 || indices_of_value_in_2D_array(board->get_walls().second, true).size() > 0)
        {
            // Support myself
            set_walls_beside_pawn(_probable_valid_next_walls, get_pawn_of_turn());
        }
        _probable_valid_next_walls.first = logicalAndBetween2DArray(_probable_valid_next_walls.first, valid_next_walls.first);
        _probable_valid_next_walls.second = logicalAndBetween2DArray(_probable_valid_next_walls.second, valid_next_walls.second);

        return _probable_valid_next_walls;
    }

    /*
    !"get_valid_next_position" function:
        ? calculates and returns the valid next positions for the pawn of the current turn on the game board.
            *Check if the valid next positions have been updated
            *Mark the valid next positions as updated
            *Initialize valid next positions
            *Set valid next positions in each direction
            *Return the valid next positions
    */
    std::vector<std::vector<bool>> get_valid_next_positions()
    {
        if (_valid_next_position_updated)
        {
            return _valid_next_positions;
        }
        _valid_next_position_updated = true;

        set2DArrayEveryElementToValue(_valid_next_positions, false);

        _set_valid_next_poistins_toward(MOVE_UP, MOVE_LEFT, MOVE_RIGHT);
        _set_valid_next_poistins_toward(MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT);
        _set_valid_next_poistins_toward(MOVE_LEFT, MOVE_UP, MOVE_DOWN);
        _set_valid_next_poistins_toward(MOVE_RIGHT, MOVE_UP, MOVE_DOWN);

        return _valid_next_positions;
    }

    // setter
    void set_turn(int value)
    {
        _turn = value;
        _valid_next_position_updated = false;
        _probable_valid_next_walls_updated = false;
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

    bool is_valid_next_move_not_considering_other_pawn(const PawnPosition &current_position, const std::vector<int> &pawn_move_tuple)
    {
        if (pawn_move_tuple == MOVE_UP)
        {
            return (current_position.get_row() > 0 &&
                    open_ways.first[current_position.get_row() - 1][current_position.get_col()]);
        }
        if (pawn_move_tuple == MOVE_DOWN)
        {
            return (current_position.get_row() < 8 &&
                    open_ways.first[current_position.get_row()][current_position.get_col()]);
        }
        else if (pawn_move_tuple == MOVE_LEFT)
        {
            return (current_position.get_col() > 0 &&
                    open_ways.second[current_position.get_row()][current_position.get_col() - 1]);
        }
        else if (pawn_move_tuple == MOVE_RIGHT)
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
    !"_set_valid_next_poistins_toward" function
        ? function is designed to calculate and set the valid next positions
        ? for the pawn of the current turn in a specific direction on the game board
            *Get the current position
            *Check if the main move is valid
            *If the main move is valid
            *If the other pawn is at the main move position
                !Check for jumping in the main direction
                !If the pawn cannot jump in the main direction
            *If the other pawn is not at the main move position

    */

    void _set_valid_next_poistins_toward(const std::vector<int> &main_move, const std::vector<int> sub_move1, const std::vector<int> sub_move2)
    {
        PawnPosition current_position = get_pawn_of_turn()->get_position();

        // converting "main_move" from vector<vector<int>> into a pair<int , int>
        std::pair<int, int> move_pair(main_move[0], main_move[1]);
        if (is_valid_next_move_not_considering_other_pawn(current_position, main_move))
        {
            PawnPosition main_move_position = current_position.new_add_move(move_pair);
            if (main_move_position.is_equals(get_pawn_of_not_turn()->get_position()))
            {
                if (is_valid_next_move_not_considering_other_pawn(main_move_position, main_move))
                {
                    PawnPosition main_main_move_position = main_move_position.new_add_move(move_pair);
                    _valid_next_positions[main_main_move_position.get_row()][main_main_move_position.get_col()] = true;
                }
                else
                {
                    // convert sub_move1 to a pair<int , int>
                    std::pair<int, int> sub_move_1_pair(sub_move1[0], sub_move1[1]);

                    if (is_valid_next_move_not_considering_other_pawn(main_move_position, sub_move1))
                    {
                        PawnPosition main_sub1_move_poition = main_move_position.new_add_move(sub_move_1_pair);
                        _valid_next_positions[main_sub1_move_poition.get_row()][main_sub1_move_poition.get_col()] = true;
                    }

                    // convert sub_move2 at a pair<int , int>
                    std::pair<int, int> sub_move_2_pair(sub_move2[0], sub_move2[1]);

                    if (is_valid_next_move_not_considering_other_pawn(main_move_position, sub_move2))
                    {
                        PawnPosition main_sub2_move_position = main_move_position.new_add_move(sub_move_2_pair);
                        _valid_next_positions[main_sub2_move_position.get_row()][main_sub2_move_position.get_col()] = true;
                    }
                }
            }
            else
            {
                _valid_next_positions[main_move_position.get_row()][main_move_position.get_col()] = true;
            }
        }
    }

    /*
!"move_pawn" function:
    ?to move a pawn in the game to a specified row and column
    ? on the game board.
    ? If the move is valid or if needCheck is false,
    ? the function updates the position of the pawn of the current turn to the specified row and column.
*/

    bool move_pawn(int row, int col, bool need_check = false)
    {
        if (need_check && !_valid_next_positions[row][col])
        {
            return false;
        }

        Pawn *pawn_of_turn = const_cast<Pawn *>(get_pawn_of_turn());
        pawn_of_turn->get_position().set_row(row);
        pawn_of_turn->get_position().set_col(col);
        if (pawn_of_turn->get_goal_row() == pawn_of_turn->get_position().get_row())
        {
            winner = pawn_of_turn;
        }
        _turn++;
        return true;
    }

    /*
    ! "is_open_way" fucntion:
        ?check if a pawn can move in a certain direction from
        ? a given position on the game board
            *current_row
            *current_col
            *pawn_move_tuple :represents the move direction for the pawn:
                *MOVE_UP
                *MOVE_DOWN
                *MOVE_LEFT
                *MOVE_RIGHT
    */
    bool is_open_way(int current_row, int current_col, const std::vector<int> &pawn_move_tuple)
    {
        if (pawn_move_tuple == MOVE_UP)
        {
            return (current_row > 0 && open_ways.first[current_row - 1][current_col]);
        }
        else if (pawn_move_tuple == MOVE_DOWN)
        {
            return (current_row < 8 && open_ways.first[current_row][current_col]);
        }
        else if (pawn_move_tuple == MOVE_LEFT)
        {
            return (current_col > 0 && open_ways.second[current_row][current_col - 1]);
        }
        else if (pawn_move_tuple == MOVE_RIGHT)
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
        ? left side of a potential horizontal wall placement on the game board.
            *Check if the column is valid
            *Check for vertical walls to the left
            *Check for vertical walls diagonally up-left and down-left
            *Check for horizontal walls further to the left
    */

    bool test_if_adjacent_to_other_wall_for_horizontal_wall_left(int row, int col)
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

    bool test_if_adjacent_to_other_wall_for_horizontal_wall_right(int row, int col)
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

    bool test_if_adjacent_to_other_wall_for_horizontal_wall_middle(int row, int col)
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

    bool test_if_connected_on_two_points_for_horizonntal_wall(int row, int col)
    {
        bool left = (col == 0 || test_if_adjacent_to_other_wall_for_horizontal_wall_left(row, col));
        bool right = (col == 7 || test_if_adjacent_to_other_wall_for_horizontal_wall_right(row, col));
        bool middle = test_if_adjacent_to_other_wall_for_horizontal_wall_middle(row, col);
        return (left && right) || (right || middle) || (middle || left);
    }

    bool test_if_adjacent_to_other_wall_for_vertical_wall_top(int row, int col)
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

    bool test_if_adjacent_to_other_wall_for_vertical_wall_bottom(int row, int col)
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

    bool test_if_adjacent_to_other_wall_for_vertical_wall_middle(int row, int col)
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

    bool test_if_connected_on_two_points_for_vertical_wall(int row, int col)
    {
        bool top = (row == 0 || test_if_adjacent_to_other_wall_for_vertical_wall_top(row, col));
        bool bottom = (row == 7 || test_if_adjacent_to_other_wall_for_vertical_wall_bottom(row, col));
        bool middle = test_if_adjacent_to_other_wall_for_vertical_wall_middle(row, col);

        return (top && bottom) || (bottom && middle) || (middle && top);
    }

    /*
    !"test_if_exist_paths_to_goal_lines_after_placing_vertical_walls" function:
    ?checks whether placing a vertical wall at the specified row and column
    ? would still allow both pawns to have a path to their respective goal lines
        *Check if the wall is connected on two points
        *Temporarily place the wal
        *Check if paths to goal lines exist
        *Remove the temporary wall
        *Return the result
    */

    bool test_if_exist_paths_to_goal_lines_after_placing_vertical_walls(int row, int col)
    {
        if (!test_if_connected_on_two_points_for_vertical_wall(row, col))
        {
            return true;
        }
        open_ways.second[row][col] = false;
        open_ways.second[row + 1][col] = false;
        bool result = exist_paths_to_goal_lines();
        open_ways.second[row][col] = true;
        open_ways.second[row + 1][col] = true;
        return result;
    }

    /*
    !"test_if_exist_paths_to_goal_lines_after_placing_horizontal_walls" function:
    ?checks whether placing a horizontal wall at the specified row and column
    ? would still allow both pawns to have a path to their respective goal lines
        *Check if the wall is connected on two points
        *Temporarily place the wal
        *Check if paths to goal lines exist
        *Remove the temporary wall
        *Return the result
    */
    bool test_if_exist_paths_to_goal_lines_after_placing_horizontal_walls(int row, int col)
    {
        if (!test_if_connected_on_two_points_for_horizonntal_wall(row, col))
        {
            return true;
        }
        open_ways.first[row][col] = false;
        open_ways.first[row][col + 1] = false;
        bool result = exist_paths_to_goal_lines();
        open_ways.first[row][col] = true;
        open_ways.first[row][col + 1] = true;
        return result;
    }

    /*
    !"is_possible_next_move" function:
        ?determines if a given move (either moving a pawn or placing a wall)
        ? is valid according to the game's rules.
        ? The move is represented as a vector of vectors, where each inner vector represents a different type of move
    */
    bool is_possible_next_move(std::vector<std::vector<int>> move)
    {
        std::vector<int> move_pawn_to = move[0];
        std::vector<int> place_horizontal_wall_at = move[1];
        std::vector<int> place_vertical_wall_at = move[2];

        if (!move_pawn_to.empty())
        {
            return this->_valid_next_positions[move_pawn_to[0]][move_pawn_to[1]];
        }
        else if (!place_horizontal_wall_at.empty())
        {
            return this->test_if_exist_paths_to_goal_lines_after_placing_horizontal_walls(place_horizontal_wall_at[0], place_horizontal_wall_at[1]);
        }
        else if (!place_vertical_wall_at.empty())
        {
            return this->test_if_exist_paths_to_goal_lines_after_placing_vertical_walls(place_vertical_wall_at[0], place_vertical_wall_at[1]);
        }
        return false;
    }

    /*
    !"update_probable_valid_next_wall_after_placing_vertical_wall" function:
        ?update the _probable_valid_next_walls data structure
        ? after placing a horizontal wall on the game board
            *Check if the wall is at the top edge of the board
            *Check if the wall is at the bottom edge of the board
            *Check if the wall is at the left edge of the board
            *Check if the wall is at the right edge of the board
    */

    void update_probable_valid_next_wall_after_placing_vertical_wall(int row, int col)
    {
        if (col >= 1)
        {
            this->_probable_next_walls.second[row][col - 1] = true;
        }
        if (col <= 6)
        {
            this->_probable_next_walls.second[row][col + 1] = true;
        }
        if (row >= 1)
        {
            this->_probable_next_walls.second[row - 1][col] = true;
            if (col >= 1)
            {
                this->_probable_next_walls.second[row - 1][col - 1] = true;
            }
            if (col <= 6)
            {
                this->_probable_next_walls.second[row - 1][col + 1] = true;
            }
            if (row >= 2)
            {
                this->_probable_next_walls.first[row - 2][col] = true;
                this->_probable_next_walls.second[row - 2][col] = true;
                if (row >= 3)
                {
                    this->_probable_next_walls.first[row - 3][col] = true;
                }
            }
        }
        if (row <= 6)
        {
            this->_probable_next_walls.second[row + 1][col] = true;
            if (col >= 1)
            {
                this->_probable_next_walls.second[row + 1][col - 1] = true;
            }
            if (col <= 6)
            {
                this->_probable_next_walls.second[row + 1][col + 1] = true;
            }
            if (row <= 5)
            {
                this->_probable_next_walls.first[row + 2][col] = true;
                this->_probable_next_walls.second[row + 2][col] = true;
                if (row <= 4)
                {
                    this->_probable_next_walls.first[row + 3][col] = true;
                }
            }
        }
    }

    void update_probable_valid_next_wall_after_placing_horizontal_wall(int row, int col)
    {
        if (row >= 1)
        {
            this->_probable_next_walls.first[row - 1][col] = true;
        }
        if (row <= 6)
        {
            this->_probable_next_walls.first[row + 1][col] = true;
        }
        if (col >= 1)
        {
            this->_probable_next_walls.first[row][col - 1] = true;
            if (row >= 1)
            {
                this->_probable_next_walls.first[row - 1][col - 1] = true;
            }
            if (row <= 6)
            {
                this->_probable_next_walls.first[row + 1][col - 1] = true;
            }
            if (col >= 2)
            {
                this->_probable_next_walls.second[row][col - 2] = true;
                this->_probable_next_walls.first[row][col - 2] = true;
                if (col >= 3)
                {
                    this->_probable_next_walls.second[row][col - 3] = true;
                }
            }
        }
        if (col <= 6)
        {
            this->_probable_next_walls.first[row][col + 1] = true;
            if (row >= 1)
            {
                this->_probable_next_walls.first[row - 1][col + 1] = true;
            }
            if (row <= 6)
            {
                this->_probable_next_walls.first[row + 1][col + 1] = true;
            }
            if (col <= 5)
            {
                this->_probable_next_walls.second[row][col + 2] = true;
                this->_probable_next_walls.first[row][col + 2] = true;
                if (col <= 4)
                {
                    this->_probable_next_walls.second[row][col + 3] = true;
                }
            }
        }
    }

    /*
    !"exist_path_to_goal_lines_for" fucntion:
        ? to determine if there exists a path from the current position
        ? of a given pawn to its goal line on the game board.
            *visited : visietd positions in search
            *pawn_move_tuples: possible movements
    */
    bool exist_path_to_goal_line_for(const Pawn &pawn)
    {

        std::vector<std::vector<bool>> visited(9, std::vector<bool>(9, false));
        std::vector<std::vector<int>> pawn_move_tuples = {MOVE_UP, MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN};

        // depth-first search (DFS) function
        std::function<bool(int, int, int)> depth_first_search = [&](int current_row, int current_col, int goal_row)
        {
            for (const auto &pawn_move_tuple : pawn_move_tuples)
            {
                if (is_open_way(current_row, current_col, pawn_move_tuple))
                {
                    int next_row = current_row + pawn_move_tuple[0];
                    int next_col = current_col + pawn_move_tuple[1];
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

    /*
    !"exist_paths_to_goal_lines" function
        ?Check if there exist paths to the goal lines for both pawns
    */

    bool exist_paths_to_goal_lines()
    {
        const Pawn *pawn1 = get_pawn_of_turn();
        const Pawn *pawn2 = get_pawn_of_not_turn();
        return (exist_path_to_goal_line_for(*pawn1) && exist_path_to_goal_line_for(*pawn2));
    }

    /*
    ! "set_walls_beside_pawn" funciton:
        ? The set_walls_beside_pawn function is used to set the possible
        ? wall positions around a given pawn on a game board
            *wall_2D_arrays.first --> horizontal walls
            *wall_2D_arrays.second --> vertical walls
            *pawn --> for accessing the position of the pawn
    */
    static void set_walls_beside_pawn(std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> &wall2DArrays, const Pawn *pawn)
    {
        int row = pawn->get_position().get_row();
        int col = pawn->get_position().get_col();
        if (row >= 1)
        {
            if (col >= 1)
            {
                wall2DArrays.first[row - 1][col - 1] = true;
                wall2DArrays.second[row - 1][col - 1] = true;
                if (col >= 2)
                {
                    wall2DArrays.first[row - 1][col - 2] = true;
                }
            }
            if (col <= 7)
            {
                wall2DArrays.first[row - 1][col] = true;
                wall2DArrays.second[row - 1][col] = true;
                if (col <= 6)
                {
                    wall2DArrays.first[row - 1][col + 1] = true;
                }
            }
            if (row >= 2)
            {
                if (col >= 1)
                {
                    wall2DArrays.second[row - 2][col - 1] = true;
                }
                if (col <= 7)
                {
                    wall2DArrays.second[row - 2][col] = true;
                }
            }
        }
        if (row <= 7)
        {
            if (col >= 1)
            {
                wall2DArrays.first[row][col - 1] = true;
                wall2DArrays.second[row][col - 1] = true;
                if (col >= 2)
                {
                    wall2DArrays.first[row][col - 2] = true;
                }
            }
            if (col <= 7)
            {
                wall2DArrays.first[row][col] = true;
                wall2DArrays.second[row][col] = true;
                if (col <= 6)
                {
                    wall2DArrays.first[row][col + 1] = true;
                }
            }
            if (row <= 6)
            {
                if (col >= 1)
                {
                    wall2DArrays.second[row + 1][col - 1] = true;
                }
                if (col <= 7)
                {
                    wall2DArrays.second[row + 1][col] = true;
                }
            }
        }
    }

private:
    std::unique_ptr<Board> board;
    Pawn *winner;
    int _turn;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> valid_next_walls;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> _probable_next_walls;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> _probable_valid_next_walls;
    bool _probable_valid_next_walls_updated;
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<bool>>> open_ways;
    std::vector<std::vector<bool>> _valid_next_positions;
    bool _valid_next_position_updated;
};
