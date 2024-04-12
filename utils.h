#ifndef UTILS_H
#define UTILS_H

#define TETRIS_UI_WIDTH 5
#define TETRIS_UI_HEIGHT 6

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "tetromino.h"

const Tetromino DECLARED_TETROMINOS[] = {
  Tetromino("I", 4, "XXXXOOOOXXXXXXXXX"),
  Tetromino("O", 2, "OOOO"),
  Tetromino("T", 3, "XOXOOOXXX"),
  Tetromino("S", 3, "XOOOOXXXX"),
  Tetromino("Z", 3, "OOXXOOXXX"),
  Tetromino("J", 3, "OXXOOOXXX"),
  Tetromino("L", 3, "XXOOOOXXX"),
};

int getRandomTetrominoIndex();

std::string timeFormat(clock_t);

struct Tetromino2D {
  Tetromino* mino;
  int x;
  int y;
};

enum CollisionType {
  OUT_OF_BOARD_X = 0,
  OUT_OF_BOARD_Y = 1,
  CONFLICT_BLOCK = 2,
  NONE = 3,
};

enum GameStatus {
  PLAYING = 0,
  LOSE = 1,
  WIN = 2,
  EXIT = 3,
};

#endif