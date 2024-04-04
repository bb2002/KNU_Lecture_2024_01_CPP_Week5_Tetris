#ifndef UTILS_H
#define UTILS_H

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20
#define TETRIS_UI_WIDTH 6
#define TETRIS_UI_HEIGHT 6

#include <string>

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

#endif