#include "QuoridorJson.h"

#include <iostream>

QuoridorGame::QuoridorGame(const std::string& jsonFilePath) {
  loadGame(jsonFilePath);
}

void QuoridorGame::loadGame(const std::string& jsonFilePath) {
  std::ifstream file(jsonFilePath);
  if (!file.is_open()) {
    std::cerr << "Unable to open file: " << jsonFilePath << std::endl;
    return;
  }

  nlohmann::json j;
  file >> j;
  parseJson(j);
}

void QuoridorGame::parseJson(const nlohmann::json& j) {
  gameId = j["gameInfo"]["gameId"];
  gameStatus = j["gameInfo"]["gameStatus"];
  currentTurn = j["gameInfo"]["currentTurn"];
  playerCount = j["settings"]["playerCount"];
  timePerMove = j["settings"]["timePerMove"];
  totalWalls = j["settings"]["totalWalls"];
  boardSize = j["settings"]["boardSize"];

  for (const auto& player : j["players"]) {
    players.push_back({player["id"],
                       player["type"],
                       {player["position"]["x"], player["position"]["y"]},
                       player["wallsLeft"],
                       player["color"]});
  }

  for (const auto& wall : j["walls"]["horizontal"]) {
    horizontalWalls.push_back({{wall["start"]["x"], wall["start"]["y"]},
                               {wall["end"]["x"], wall["end"]["y"]}});
  }

  for (const auto& wall : j["walls"]["vertical"]) {
    verticalWalls.push_back({{wall["start"]["x"], wall["start"]["y"]},
                             {wall["end"]["x"], wall["end"]["y"]}});
  }
}

std::string QuoridorGame::getGameId() const { return gameId; }

std::string QuoridorGame::getGameStatus() const { return gameStatus; }

int QuoridorGame::getCurrentTurn() const { return currentTurn; }

int QuoridorGame::getPlayerCount() const { return playerCount; }

int QuoridorGame::getTimePerMove() const { return timePerMove; }

int QuoridorGame::getTotalWalls() const { return totalWalls; }

int QuoridorGame::getBoardSize() const { return boardSize; }

std::vector<player> QuoridorGame::getPlayers() const { return players; }

std::vector<wall> QuoridorGame::getHorizontalWalls() const {
  return horizontalWalls;
}

std::vector<wall> QuoridorGame::getVerticalWalls() const {
  return verticalWalls;
}
