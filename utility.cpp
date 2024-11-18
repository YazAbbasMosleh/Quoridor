// Function for initialization of the board game
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

// pawn movement representation
const std::vector<int> MOVE_UP = {-1, 0};
const std::vector<int> MOVE_DOWN = {1, 0};
const std::vector<int> MOVE_RIGHT = {0, 1};
const std::vector<int> MOVE_LEFT = {0, -1};

std::vector<std::vector<bool>> array_initialization(int num_row, int num_col, bool initial_value)
{
    return std::vector<std::vector<bool>>(num_row, std::vector<bool>(num_col, initial_value));
}

// setting every element of the game board to a value
void valuing_2D_array(std::vector<std::vector<bool>> &arr2D_original, bool value)
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
std::vector<std::vector<int>> game_board_copying(const std::vector<std::vector<int>> &arr2D_original)
{
    std::vector<std::vector<int>> arr2D_copy = arr2D_original;
    return arr2D_copy;
}

// logical operation between 2 states
std::vector<std::vector<bool>> array_2D_and_operation(const std::vector<std::vector<bool>> &arr2D_original_A, const std::vector<std::vector<bool>> &arr2D_original_B)
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

std::vector<std::pair<int, int>> index_of_value_in_2D_array(const std::vector<std::vector<bool>> &arr2D, bool value)
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

std::vector<std::vector<bool>> cloned_array(const std::vector<std::vector<bool>> &arr)
{
    std::vector<std::vector<bool>> arr_cloned;
    arr_cloned.reserve(arr.size()); 

    for (const auto &row : arr)
    {
        arr_cloned.emplace_back(row); 
    }

    return arr_cloned;
}


//fucntion to get a random index from an array
template<typename T>
int random_index(const std::vector<T>& arr)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0 , arr.size()-1);
    
    return dis(gen);
}

//fucntion to get a random element from an array
template<typename T>
T random_element(const std::vector<T>& arr)
{
    int index = random_index(arr);
    return arr[index];
}