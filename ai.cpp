#include "AI.h"

std::vector<std::pair<int, int>> AI::chooseNextMove(
    Player& player, std::unique_ptr<GameRule> game) {
  clock_t d0 = clock();

  if (game->get_turn() < 1) {
    Position nextPosition =
        chooseShortestPathNextPawnPosition(game->get_board());
    auto pawnMoveTuple = nextPosition.displacement_from(player.get_position());
    if (pawnMoveTuple.second == 0) {
      if (!forWorker) {
        return {{nextPosition.get_row(), nextPosition.get_col()}, {}, {}};
      }
    }

    Mcts mcts(game.release(), uctConst);

    if (forWorker) {
      int nSearch = 50;
      int nBatch = (numOfMCTSSimulations + nSearch - 1) /
                   nSearch;  // ceil(numOfMCTSSimulations / nSearch)
      // Implement your postMessage equivalent here
      for (int i = 0; i < nSearch; ++i) {
        mcts.search(nBatch);
        // Implement your postMessage equivalent here
      }
    } else {
      mcts.search(numOfMCTSSimulations);
    }

    auto best = mcts.best_move_selection();
    std::vector<std::pair<int, int>> bestMove = best.first;
    double winRate = best.second;

    if ((game->get_turn() < 6 && player.get_position().get_col() == 4) ||
        winRate < 0.1) {
      bool rightMove = false;
      std::vector<Position> nextPositions =
          chooseShortestPathNextPawnPositionsThoroughly(player);
      for (const auto& nextPosition : nextPositions) {
        if (bestMove[0] ==
            std::make_pair(nextPosition.get_row(), nextPosition.get_col())) {
          rightMove = true;
          break;
        }
      }
      if (!rightMove) {
        std::cout << "original move: " << bestMove[0].first << ", "
                  << bestMove[0].second << std::endl;
        Position nextPosition = random_element(nextPositions);
        bestMove = {{nextPosition.get_row(), nextPosition.get_col()}, {}, {}};
      }
    }

    if (game->get_turn() < 5) {
      if (player.get_position().get_col() == 4 &&
          player.get_position().get_col() == 6 &&
          static_cast<double>(rand()) / RAND_MAX < 0.5) {
        std::vector<std::vector<std::pair<std::nullptr_t, std::pair<int, int>>>>
            bestMoves = {{nullptr, {5, 3}},
                         {nullptr, {5, 4}},
                         {nullptr, {4, 3}},
                         {nullptr, {4, 4}}};
        std::cout << "original move: " << bestMove[0].first << ", "
                  << bestMove[0].second << std::endl;
        bestMove = random_element(bestMoves);
      }
      if (player.get_position().get_col() == 4 &&
          player.get_position().get_row() == 2 &&
          static_cast<double>(rand()) / RAND_MAX < 0.5) {
        std::vector<std::vector<std::pair<std::nullptr_t, std::pair<int, int>>>>
            bestMoves = {{nullptr, {2, 3}},
                         {nullptr, {2, 4}},
                         {nullptr, {3, 3}},
                         {nullptr, {3, 4}}};
        std::cout << "original move: " << bestMove[0].first << ", "
                  << bestMove[0].second << std::endl;
        bestMove = random_element(bestMoves);
      }
    }

    clock_t d1 = clock();
    double elapsed_time = double(d1 - d0) / CLOCKS_PER_SEC;
    std::cout << "Time taken by AI for " << numOfMCTSSimulations
              << " rollouts, c=" << uctConst << ": " << elapsed_time << " sec"
              << std::endl;

    if (aiDevelopMode) {
      std::cout << "Descend maxWinRateChild" << std::endl;
      auto node = mcts.get_root();
      int i = 1;
      while (!node->get_children().empty()) {
        node = node.maxWinRateChild;
        std::cout << i << " " << node.move << " " << node.winRate << " "
                  << node.numWins << " " << node.numSims << std::endl;
        ++i;
      }
      std::cout << "Descend maxSimsChild" << std::endl;
      node = mcts.root;
      i = 1;
      while (!node.children.empty()) {
        node = node.maxSimsChild;
        std::cout << i << " " << node.move << " " << node.winRate << " "
                  << node.numWins << " " << node.numSims << std::endl;
        ++i;
      }
      std::cout << "Max depth: " << MCTS::maxDepth(mcts.root) << std::endl;
      std::cout << "Estimated maxWinRateChild win rate: "
                << mcts.root.maxWinRateChild.winRate << std::endl;
      std::cout << "Estimated maxSimsChild win rate: " << winRate << std::endl;
    } else {
      std::cout << "Estimated AI win rate: " << winRate << std::endl;
    }

    return bestMove;
  }
}
