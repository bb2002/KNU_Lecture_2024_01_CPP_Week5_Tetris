#include "tetromino.h"

Tetromino::Tetromino(std::string name, int size, std::string shape) : name_(name), size_(size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      this->shape_[i][j] = shape[size * i + j] == 'O';
    }
  }
  this->original_ = this;
}

Tetromino::Tetromino(std::string name, int size, bool shape[MAX_SIZE][MAX_SIZE]) : name_(name), size_(size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      this->shape_[i][j] = shape[i][j];
    }
  }
  this->original_ = this;
}

Tetromino Tetromino::rotatedCW() {
  bool shape[MAX_SIZE][MAX_SIZE];
  int size = this->size_;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      shape[j][size - j - 1] = this->shape_[i][j];
    }
  }

  Tetromino rotated(this->name_, this->size_, shape);
  return rotated;
}

Tetromino Tetromino::rotatedCCW() {
 bool shape[MAX_SIZE][MAX_SIZE];
  int size = this->size_;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      shape[size - 1 - j][i] = this->shape_[i][j];
    }
  }

  Tetromino rotated(this->name_, this->size_, shape);
  return rotated;
}

