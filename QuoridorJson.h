#ifndef QUORIDOR_JSON_H
#define QUORIDOR_JSON_H

#include <fstream>
#include <json.hpp>
#include <string>
#include <vector>

struct position {
  int x;
  int y;
};

struct player {
  std::string id;
  std::string type;
  position position;
  int wallsLeft;
  std::string color;
};

struct wall {
  position start;
  position end;
};

class QuoridorGame {
 public:
  QuoridorGame(const std::string& jsonFilePath);

  void loadGame(const std::string& jsonFilePath);

  // Getters for game parameters
  std::string getGameId() const;
  std::string getGameStatus() const;
  int getCurrentTurn() const;
  int getPlayerCount() const;
  int getTimePerMove() const;
  int getTotalWalls() const;
  int getBoardSize() const;
  std::vector<player> getPlayers() const;
  std::vector<wall> getHorizontalWalls() const;
  std::vector<wall> getVerticalWalls() const;

 private:
  void parseJson(const nlohmann::json& j);

  std::string gameId;
  std::string gameStatus;
  int currentTurn;
  int playerCount;
  int timePerMove;
  int totalWalls;
  int boardSize;
  std::vector<player> players;
  std::vector<wall> horizontalWalls;
  std::vector<wall> verticalWalls;
};

#endif  // QUORIDOR_GAME_H
