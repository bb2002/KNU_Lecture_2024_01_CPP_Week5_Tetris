#include "game.h"
#include "console/console.h"
#include "utils.h"
#include "tetromino.h"
#include <string>

Game::Game() {
  // Tetromino test("T", 3, "XOXOOOXXX");
  // this->tetrominos[this->tetrominoSize++] = test.rotatedCCW();
}

void Game::update() {
  ++tick;
  if (tick % 60 == 0) {
    this->update1Second();
    return;
  }
}

void Game::update1Second() {
  if (this->currentMino == NULL) {
    int rd = getRandomTetrominoIndex();
    this->currentMino = new Tetromino(
      TETROMINO_NAME[rd],
      TETROMINO_SIZE[rd],
      TETROMINO_SHAPE[rd]
    );
    this->tetrominos[this->tetrominoSize++] = this->currentMino;
  }
}

void Game::draw() {
  this->drawUI();
  this->drawTetrominos();
}

void Game::drawUI() {
  // 화면 생성
  console::drawBox(0, 0, TETRIS_WIDTH,TETRIS_HEIGHT);
  
  // Next 상자 생성
  console::drawBox(11, 0, 11 + TETRIS_UI_WIDTH, TETRIS_UI_HEIGHT);
  console::draw(12, 0, "Next");

  // Hold 상자 생성
  console::drawBox(18, 0, 18 + TETRIS_UI_WIDTH, TETRIS_UI_HEIGHT);
  console::draw(19, 0, "Hold");
}

void Game::drawTetrominos() {
  // Draw Tetrominos
  for (int i = 0; i < this->tetrominoSize; ++i) {
    this->tetrominos[i]->drawAt(BLOCK_STRING, 1, 1);
  }
}

bool Game::shouldExit() {
  return false;
}