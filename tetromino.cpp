#include "tetromino.h"

Tetromino::Tetromino(std::string name, int size, std::string shape) : name_(name), size_(size) {
  int index = 0;
  for (int y = 0; y < this->size(); ++y) {
    for (int x = 0; x < this->size(); ++x) {
      this->shape_[y][x] = shape[index++] == 'O';
    }
  }

  this->original_ = this;
}

Tetromino Tetromino::rotatedCW() {
  bool temp[MAX_SIZE][MAX_SIZE]; // 임시 배열

  // 2차 배열을 시계 방향으로 회전
  for (int i = 0; i < this->size(); ++i) {
    for (int j = 0; j < this->size(); ++j) {
      temp[this->size() - 1 - j][i] = this->shape_[i][j];
    }
  }

  // 회전된 배열을 직렬화
  std::string serialized = "";
  for (int y = 0; y < this->size(); ++y) {
    for (int x = 0; x < this->size(); ++x) {
      serialized += temp[y][x] ? "O" : "X";
    }
  }

  return Tetromino(this->name(), this->size(), serialized);
}

Tetromino Tetromino::rotatedCCW() {
  bool temp[MAX_SIZE][MAX_SIZE]; // 임시 배열

  // 2차 배열을 반시계 방향으로 회전
  for (int i = 0; i < this->size_; ++i) {
    for (int j = 0; j < this->size_; ++j) {
      temp[j][this->size() - i - 1] = this->shape_[i][j];
    }
  }

  // 회전된 배열을 직렬화
  std::string serialized = "";
  for (int y = 0; y < this->size(); ++y) {
    for (int x = 0; x < this->size(); ++x) {
      serialized += temp[y][x] ? "O" : "X";
    }
  }

  return Tetromino(this->name(), this->size(), serialized);
}

void Tetromino::drawAt(std::string s, int absoluteX, int absoluteY) {
  for (int y = absoluteY; y < absoluteY + this->size(); ++y) {
    for (int x = absoluteX; x < absoluteX + this->size(); ++x) {
      if (this->shape_[y - absoluteY][x - absoluteX]) {
        console::draw(x, y, s);
      }
    }
  }
}