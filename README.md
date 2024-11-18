# Quoridor Auto-Player (Back-End) using Monte-Carlo Tree Search (MCTS)

This project aims to develop an auto-player for the **Quoridor** board game using the **Monte-Carlo Tree Search (MCTS)** algorithm. The project serves as the final project for the **Advanced Programming** course at **Amirkabir University of Technology**.

## Project Overview

**Quoridor** is a two-player strategy board game where players take turns to either move their pawn or place a wall to block the opponent’s movement. The goal is to be the first to reach the opponent's side of the board.

In this project, we developed an auto-player for the Quoridor game using the **Monte-Carlo Tree Search (MCTS)** algorithm. The AI can make optimal decisions by simulating potential future game states and choosing the most promising moves based on randomized simulations.

### Features:
- **Monte-Carlo Tree Search (MCTS)** for decision-making.
- Simulates multiple game scenarios to find the best possible move.
- AI plays as one of the players in a Quoridor game, against either another AI or a human.

## Installation

To run this project locally, follow these steps:

1. **Clone the repository:**
   ```bash
   git clone git@github.com:YazAbbasMosleh/Quoridor.git
   cd Quoridor

   # Quoridor Auto-Player (Back-End) using Monte-Carlo Tree Search (MCTS)

This project aims to develop an auto-player for the **Quoridor** board game using the **Monte-Carlo Tree Search (MCTS)** algorithm. The project serves as the final project for the **Advanced Programming** course at **Amirkabir University of Technology**.

## Table of Contents
- [Project Overview](#project-overview)
- [Installation](#installation)
- [Usage](#usage)
- [Algorithm](#algorithm)
- [Project Structure](#project-structure)
- [Contributors](#contributors)
- [License](#license)

## Project Overview

**Quoridor** is a two-player strategy board game where players take turns to either move their pawn or place a wall to block the opponent’s movement. The goal is to be the first to reach the opponent's side of the board.

In this project, we developed an auto-player for the Quoridor game using the **Monte-Carlo Tree Search (MCTS)** algorithm. The AI can make optimal decisions by simulating potential future game states and choosing the most promising moves based on randomized simulations.

### Features:
- **Monte-Carlo Tree Search (MCTS)** for decision-making.
- Simulates multiple game scenarios to find the best possible move.
- AI plays as one of the players in a Quoridor game, against either another AI or a human.

## Installation

To run this project locally, follow these steps:

1. **Clone the repository:**
   ```bash
   git clone git@github.com:YazAbbasMosleh/Quoridor.git
   cd Quoridor
## Usage

Once compiled, you can run the program to start a **Quoridor** game with the AI as one of the players. The AI will use the **Monte-Carlo Tree Search (MCTS)** algorithm to select the best possible moves based on the current state of the game.

### Running the Game

After compiling the project, execute the following command to start the game:

```bash
./quoridor_auto_player


## Algorithm

### Monte-Carlo Tree Search (MCTS)

Monte-Carlo Tree Search (MCTS) is a decision-making algorithm used for games and decision processes that involve uncertainty. It works by simulating many possible game outcomes and using these simulations to build a tree of possible moves. Each node in the tree represents a game state, and the branches represent the possible moves.

### Four Key Steps in MCTS:

1. **Selection**: 
   - Starting from the root node, select a child node to explore based on a selection policy (typically UCT - Upper Confidence bounds applied to Trees).
   
2. **Expansion**: 
   - If the selected node is not a terminal state (i.e., the game is not finished), generate a new child node corresponding to a valid move.

3. **Simulation**: 
   - Simulate a random playout from the new node to a terminal state (i.e., the game ends).

4. **Backpropagation**: 
   - Update the nodes on the path from the expanded node back to the root with the result of the simulation.

This algorithm allows the AI to evaluate the best possible move by performing multiple simulations in parallel, enabling it to play optimally against human or computer opponents.


