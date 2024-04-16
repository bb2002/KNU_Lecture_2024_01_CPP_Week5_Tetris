
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <array>
#include <string>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <ctime>
#include "tetromino.h"
#include "utils.h"
#include "console/console.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 10

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_HEIGHT][BOARD_WIDTH] = { {false,}, };

  // 프레임레이트 계산을 위한 Tick
  int tick = 0;

  // 렌더링을 위한 테트로미노 배열
  std::vector<Tetromino2D*> tetrominos;

  // 현재 추락중인 테트로미노
  Tetromino2D* currentMino = NULL;

  // 다음 테트로미노
  Tetromino2D* nextMino = NULL;

  // Hold 테트로미노
  Tetromino2D* holdMino = NULL;

  // 현재 플레이어가 누르는 키
  console::Key pressedKey = console::Key::K_NONE;

  // 홀드를 사용 할 수 있는가?
  bool canUseHold = true;

  // 현재 게임 상태
  GameStatus status = GameStatus::PLAYING;

  // 현재 점수
  int score = 0;

  // 게임 시작 시간
  clock_t startAt = clock();

  // 현재 플레이한 시간
  clock_t playTime = clock();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();

private:
  // (추가) 게임의 UI 를 그린다
  void drawUI();

  // (추가) 테트로미노를 그린다
  void drawTetrominos();

  // (추가) 대상 테트리미노의 움직임 처리
  void moveTetromino(int tick);

  // (추가) 테트리미노 소환
  Tetromino2D* spawnTetromino(int x, int y);

  // (추가) 입력을 감지하는 함수
  void inputListener();

  // (추가) 테트리미노가 해당 위치에 렌더링 될 때 충돌이 생기는지 검사
  CollisionType collisionTester(
    Tetromino& mino, 
    int simulateX, 
    int simulateY, 
    Tetromino* excludeMino = NULL
  );

  // (추가) 테트리미노가 끝까지 내려가려면 얼마나 더 내려가야 하는지 찾기
  int findRemainingDownwardDistance(
    Tetromino& mino, 
    int simulateX, 
    int simulateY
  );

  // (추가) 한 줄이 다 완성되었는지 감지
  int detectListCompleted();

  // (추가) 해당 라인을 비우고 점수 증가
  void removeLineAndGetPoint(int line);

  // (추가) DROP_DELAY에 한번씩 호출된다.
  void updateSlowly();
};
#endif