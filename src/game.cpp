#include "game.hpp"
#include "blocks.cpp"
#include <stdlib.h>

Game::Game() {
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    hasHeldBlock = false;
    canHold = true;
    lockStartTime = -1.0;
}

Block Game::GetRandomBlock() {
    if (blocks.empty()) {
        blocks = GetAllBlocks();
    }
    int randInt = rand() % blocks.size();
    Block block = blocks[randInt];
    blocks.erase(blocks.begin() + randInt);
    return block;
}

std::vector<Block> Game::GetAllBlocks() {
    return {IBlock(), JBlock(), LBlock(), OBlock(),
            SBlock(), TBlock(), ZBlock()};
}

Block Game::GetBlockById(int id) {
    switch (id) {
    case 1:
        return SBlock();
    case 2:
        return ZBlock();
    case 3:
        return LBlock();
    case 4:
        return OBlock();
    case 5:
        return TBlock();
    case 6:
        return IBlock();
    case 7:
        return JBlock();
    default:
        return LBlock();
    }
}

void Game::Draw() {
    grid.Draw();
    Block ghostBlock = GetGhostBlock();
    ghostBlock.DrawGhost(11, 11);
    currentBlock.Draw(11, 11);
    switch (nextBlock.id) {
    case 6:
        nextBlock.Draw(255, 250);
        break;
    case 4:
        nextBlock.Draw(255, 240);
        break;
    default:
        nextBlock.Draw(270, 230);
    }
    if (hasHeldBlock) {
        switch (holdBlock.id) {
        case 6:
            holdBlock.Draw(255, 475);
            break;
        case 4:
            holdBlock.Draw(255, 465);
            break;
        default:
            holdBlock.Draw(270, 455);
        }
    }
}

Block Game::GetGhostBlock() {
    Block ghostBlock = currentBlock;

    while (true) {
        ghostBlock.Move(1, 0);
        std::vector<Position> tiles = ghostBlock.GetCellPositions();
        bool blocked = false;

        for (Position tile : tiles) {
            if (grid.IsCellOutside(tile.row, tile.col) ||
                !grid.IsCellEmpty(tile.row, tile.col)) {
                blocked = true;
                break;
            }
        }

        if (blocked) {
            ghostBlock.Move(-1, 0);
            break;
        }
    }

    return ghostBlock;
}

void Game::HandleInput() {
    int keyPressed = GetKeyPressed();
    if (gameOver && keyPressed != 0) {
        gameOver = false;
        Reset();
    }

    switch (keyPressed) {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0, 1);
        break;
    case KEY_SPACE:
        HardDropBlock();
        UpdateScore(0, 2);
        break;
    case KEY_C:
        HoldBlock();
        break;
    case KEY_UP:
        RotateBlock();
        break;
    default:
        break;
    }
}

void Game::MoveBlockLeft() {
    if (!gameOver) {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || !BlockFits()) {
            currentBlock.Move(0, 1);
        } else {
            ResetLockTimer();
        }
    }
}

void Game::MoveBlockRight() {
    if (!gameOver) {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || !BlockFits()) {
            currentBlock.Move(0, -1);
        } else {
            ResetLockTimer();
        }
    }
}

void Game::MoveBlockDown() {
    if (!gameOver) {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || !BlockFits()) {
            currentBlock.Move(-1, 0);
            double currentTime = GetTime();
            if (lockStartTime < 0.0) {
                lockStartTime = currentTime;
            } else if (currentTime - lockStartTime >= lockDelay) {
                LockBlock();
            }
        } else {
            lockStartTime = -1.0;
        }
    }
}

void Game::HardDropBlock() {
    if (!gameOver) {
        while (!IsBlockOutside() && BlockFits()) {
            currentBlock.Move(1, 0);
        }
        currentBlock.Move(-1, 0);
        LockBlock();
    }
}

void Game::RotateBlock() {
    if (!gameOver) {
        currentBlock.Rotate();
        if (IsBlockOutside() || !BlockFits()) {
            currentBlock.UndoRotation();
        } else {
            ResetLockTimer();
        }
    }
}

void Game::HoldBlock() {
    if (!canHold || gameOver) {
        return;
    }
    int heldId = currentBlock.id;
    if (hasHeldBlock) {
        currentBlock = GetBlockById(holdBlock.id);
    } else {
        currentBlock = GetRandomBlock();
    }
    holdBlock = GetBlockById(heldId);
    hasHeldBlock = true;
    canHold = false;
    lockStartTime = -1.0;
}

bool Game::IsBlockOutside() {
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position tile : tiles) {
        if (grid.IsCellOutside(tile.row, tile.col)) {
            return true;
        }
    }
    return false;
}

void Game::LockBlock() {
    lockStartTime = -1.0;
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position tile : tiles) {
        grid.grid[tile.row][tile.col] = currentBlock.id;
    }
    currentBlock = nextBlock;
    canHold = true;
    if (BlockFits() == false) {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    UpdateScore(rowsCleared, 0);
}

bool Game::BlockFits() {
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position tile : tiles) {
        if (!grid.IsCellEmpty(tile.row, tile.col)) {
            return false;
        }
    }
    return true;
}

bool Game::IsBlockGrounded() {
    currentBlock.Move(1, 0);
    bool grounded = IsBlockOutside() || !BlockFits();
    currentBlock.Move(-1, 0);
    return grounded;
}

void Game::ResetLockTimer() {
    lockStartTime = IsBlockGrounded() ? GetTime() : -1.0;
}

void Game::Reset() {
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
    hasHeldBlock = false;
    canHold = true;
    lockStartTime = -1.0;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints) {
    switch (linesCleared) {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
    case 3:
        score += 500;
        break;
    default:
        break;
    }

    score += moveDownPoints;
}
