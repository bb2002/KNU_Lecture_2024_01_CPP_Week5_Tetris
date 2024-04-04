
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40
#define MAX_TETROMINO_SIZE 1000

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];

  // 프레임레이트 계산을 위한 Tick
  int tick = 0;

  // 렌더링을 위한 테트로미노 배열
  Tetromino* tetrominos[MAX_TETROMINO_SIZE];
  int tetrominoSize = 0;

  // 현재 추락중인 테트로미노
  Tetromino* currentMino = NULL;

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // (추가) 1초에 한번씩 호출된다.
  void update1Second();

  // 게임 화면을 그린다.
  void draw();

  // (추가) 게임의 UI 를 그린다
  void drawUI();

  void drawTetrominos();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif