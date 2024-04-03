#include "utils.h"
#include "tetromino.h"

TetrominoMapper::TetrominoMapper() {
  this->next = nullptr; 
}

TetrominoMapper::TetrominoMapper(TetrominoMapper* next, Tetromino& data) {
  this->next = next;
  this->data = data;
}