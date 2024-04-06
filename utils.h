#ifndef UTILS_H
#define UTILS_H

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20
#define TETRIS_UI_WIDTH 6
#define TETRIS_UI_HEIGHT 6

#include <string>
#include "tetromino.h"

const std::string TETROMINO_NAME[] = { "I", "O", "T", "S", "Z", "J", "L" };

const std::string TETROMINO_SHAPE[] = {
  "XXXXOOOOXXXXXXXX",
  "XXXXOOXOO",
  "XXXXOXOOO",
  "XOOOOXXXX",
  "OOXXOOXXX",
  "OXXOOOXXX",
  "XXOOOOXXX"
};

const int TETROMINO_SIZE[] = { 4,3,3,3,3,3,3 };

int getRandomTetrominoIndex();

struct Tetromino2D {
  Tetromino mino;
  int x;
  int y;
};

enum CollisionType {
  OUT_OF_BOARD_X = 0,
  OUT_OF_BOARD_Y = 1,
  CONFLICT_BLOCK = 2,
  NONE = 3,
};

#endif