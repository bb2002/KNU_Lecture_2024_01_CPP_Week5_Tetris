#include "tetromino.h"
#include "console/console.h"

Tetromino::Tetromino() {}

Tetromino::Tetromino(std::string name, int size, std::string shape) : name_(name), size_(size) {
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      this->shape_[j][i] = shape[size * i + j] == 'O';
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
  bool temp[MAX_SIZE][MAX_SIZE]; // 임시 배열

  for (int i = 0; i < this->size_; ++i) {
      for (int j = 0; j < this->size_; ++j) {
          temp[j][this->size_ - i - 1] = this->shape_[i][j]; // 임시 배열에 회전된 요소 저장
      }
  }

  Tetromino rotated(this->name_, this->size_, temp);
  rotated.original_ = this->original();
  return rotated;
}

Tetromino Tetromino::rotatedCCW() {
  bool temp[MAX_SIZE][MAX_SIZE];
  int size = this->size_;

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      temp[size - 1 - j][i] = this->shape_[i][j];
    }
  }

  Tetromino rotated(this->name_, this->size_, temp);
  rotated.original_ = this->original();
  return rotated;
}

void Tetromino::drawAt(std::string s, int x, int y) {
  for (int i = x; i < x + this->size_; ++i) {
    for (int j = y; j < y + this->size_; ++j) {
      if (this->shape_[i - x][j - y]) {
        console::draw(i, j, s);
      }
    } 
  }
}