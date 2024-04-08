#ifndef UTILS_H
#define UTILS_H

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20
#define TETRIS_UI_WIDTH 6
#define TETRIS_UI_HEIGHT 6

#include <string>
#include "tetromino.h"

const Tetromino DECLARED_TETROMINOS[] = {
  Tetromino("I", 4, "XXXXOOOOXXXXXXXX"),
  Tetromino("O", 2, "OOOO"),
  Tetromino("T", 3, "XOXOOOXXX"),
  Tetromino("S", 3, "XOOOOXXXX"),
  Tetromino("Z", 3, "OOXXOOXXX"),
  Tetromino("J", 3, "OXXOOOXXX"),
  Tetromino("L", 3, "XXOOOOXXX"),
};

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