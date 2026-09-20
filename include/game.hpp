#pragma once
#include "block.hpp"
#include "grid.hpp"
#include <vector>

class Game {
  public:
    Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    bool gameOver;
    int score;

  private:
    Block GetRandomBlock();
    Block GetBlockById(int id);
    Block GetGhostBlock();
    std::vector<Block> GetAllBlocks();
    void MoveBlockLeft();
    void MoveBlockRight();
    void HardDropBlock();
    void HoldBlock();
    Grid grid;
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int linesCleared, int moveDownPoints);
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Block holdBlock;
    bool hasHeldBlock;
    bool canHold;
};
