#include "grid.hpp"
#include "colors.hpp"
#include <iostream>

Grid::Grid() {
    numRows = 20;
    numCols = 10;
    cellSize = 30;
    Initialize();
    colors = GetCellColors();
}

void Grid::Initialize() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            grid[row][col] = 0;
        }
    }
}

void Grid::Print() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            std::cout << grid[row][col] << " ";
        }
        std::cout << std::endl;
    }
}

void Grid::Draw() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            int cellVal = grid[row][col];
            DrawRectangle(col * cellSize + 11, row * cellSize + 11,
                          cellSize - 1, cellSize - 1, colors[cellVal]);
        }
    }
}

bool Grid::IsCellOutside(int row, int col) {
    return !(row >= 0 && row < numRows && col >= 0 && col < numCols);
}

bool Grid::IsCellEmpty(int row, int col) { return grid[row][col] == 0; }

bool Grid::IsRowFull(int row) {
    for (int col = 0; col < numCols; col++) {
        if (grid[row][col] == 0) {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row) {
    for (int col = 0; col < numCols; col++) {
        grid[row][col] = 0;
    }
}

void Grid::MoveRowDown(int row, int numRowsDown) {
    for (int col = 0; col < numCols; col++) {
        grid[row + numRowsDown][col] = grid[row][col];
        grid[row][col] = 0;
    }
}

int Grid::ClearFullRows() {
    int complete = 0;
    for (int row = numRows - 1; row >= 0; row--) {
        if (IsRowFull(row)) {
            ClearRow(row);
            complete++;
        } else if (complete > 0) {
            MoveRowDown(row, complete);
        }
    }
    return complete;
}
