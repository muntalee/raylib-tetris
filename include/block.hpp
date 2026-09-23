#pragma once
#include "colors.hpp"
#include "position.hpp"
#include <map>
#include <vector>

class Block {
  public:
    Block();
    void Draw(int offsetX, int offsetY);
    void DrawGhost(int offsetX, int offsetY);
    void Move(int rows, int cols);
    std::vector<Position> GetCellPositions();
    void Rotate();
    void UndoRotation();
    int id;
    std::map<int, std::vector<Position>> cells;

  private:
    int cellSize;
    int rotationState;
    std::vector<Color> colors;
    int rowOffset;
    int colOffset;
};
