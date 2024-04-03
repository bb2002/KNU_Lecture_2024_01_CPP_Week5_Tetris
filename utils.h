#ifndef DECLARED_H
#define DECLARED_H

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20
#define TETRIS_UI_WIDTH 6
#define TETRIS_UI_HEIGHT 6

#include "tetromino.h"

class TetrominoMapper {
public:
  TetrominoMapper* next;
  Tetromino data;

  TetrominoMapper();

  TetrominoMapper(TetrominoMapper*, Tetromino&);
};

#endif