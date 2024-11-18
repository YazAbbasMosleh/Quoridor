#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <queue>
#include <variant>
#include <vector>

#include "gameboard.h"
#include "gamerule.h"
#include "mcts.h"
#include "node.h"
#include "player.h"
#include "position.h"
// #include "utilities.h"
// Assume that these utility functions and classes are defined somewhere
// like randomChoice, create2DArrayInitializedTo, indicesOfValueIn2DArray, etc.

class AI {
 public:
  AI(int numOfMCTSSimulations, double uctConst, bool aiDevelopMode = false,
     bool forWorker = false)
      : numOfMCTSSimulations(numOfMCTSSimulations),
        uctConst(uctConst),
        aiDevelopMode(aiDevelopMode),
        forWorker(forWorker){};

  std::vector<std::pair<int, int>> chooseNextMove(
      Player& player, std::unique_ptr<GameRule> game);

 private:
  int numOfMCTSSimulations;
  double uctConst;
  bool aiDevelopMode;
  bool forWorker;
};
