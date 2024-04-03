#include "game.h"
#include "console/console.h"
#include "declared.h"

Game::Game() {

}

void Game::update() {
  ++tick;
  if (tick % 60 == 0) {
    this->update1Second();
    return;
  }
}

void Game::update1Second() {
  
}

void Game::draw() {
  this->drawUI();

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


bool Game::shouldExit() {
  return false;
}