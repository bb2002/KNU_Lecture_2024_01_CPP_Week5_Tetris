#include "game.h"

Game::Game() {
  srand(1); 
  this->currentMino = this->spawnTetromino(1,1);
  this->tetrominos.push_back(this->currentMino);

  // 다음 테트리미노와 현재 테트리미노를 동시에 초기화하면 
  // 렌덤이 같은 값을 반환할 확률이 커져서
  // 시드를 다르게 준다.
  srand(time(NULL));
  this->nextMino = this->spawnTetromino(13, 1);
}

void Game::update() {
  ++tick;
  this->inputListener();
  this->moveTetromino(tick);

  if (tick == 1 || tick % DROP_DELAY == 0) {
    this->updateSlowly();
    return;
  }
}

void Game::updateSlowly() {
  if (this->nextMino == NULL) {
    this->nextMino = this->spawnTetromino(13, 1);
  }

  if (this->currentMino == NULL) {
    this->currentMino = this->nextMino;
    this->nextMino = NULL;
    this->currentMino->x = 5;
    this->currentMino->y = 1;
    this->tetrominos.push_back(this->currentMino);
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
  if (this->nextMino != NULL) {
    // 
    this->nextMino->mino.drawAt(BLOCK_STRING, 13, 1);
  }

  if (this->currentMino != NULL) {
    this->currentMino->mino.drawAt(
      SHADOW_STRING,
      this->currentMino->x,
      this->currentMino->y + this->findRemainingDownwardDistance(
        this->currentMino->mino,
        this->currentMino->x,
        this->currentMino->y
      )
    );
  }

  if (this->holdMino != NULL) {
    this->holdMino->mino.drawAt(
      BLOCK_STRING,
      20, 1
    );
  }

  // Draw Tetrominos
  for (Tetromino2D* item : this->tetrominos) {
    item->mino.drawAt(BLOCK_STRING, item->x, item->y);
  }
}

void Game::inputListener() {
  console::Key KEYS[] = {
    console::Key::K_OTHER,
    console::Key::K_LEFT,
    console::Key::K_RIGHT,
    console::Key::K_UP,
    console::Key::K_DOWN,
    console::Key::K_ESC,
    console::Key::K_SPACE,
    console::Key::K_Z,
    console::Key::K_X,
    console::Key::K_ENTER,
  };

  for (int i = 0; i < 10; ++i) {
    if (console::key(KEYS[i])) {
      this->pressedKey = KEYS[i];
      return;
    }
  }
}

void Game::moveTetromino(int tick) {
  if (this->currentMino == NULL) {
    // 움직일 테트리미노가 없는 경우
    return;
  }

  int x = 0;
  int y = 0;
  int speed = 1;
  if (this->pressedKey == console::Key::K_LEFT) {
    x = -1;
  }

  if (this->pressedKey == console::Key::K_RIGHT) {
    x = 1;
  }

  if (this->pressedKey == console::Key::K_SPACE && this->canUseHold) {
    this->canUseHold = false;

    // 렌더 배열에서 현재 미노를 제거
    std::remove(this->tetrominos.begin(), this->tetrominos.end(), this->currentMino);

    if (this->holdMino == NULL) {
      this->holdMino = this->currentMino;
      this->holdMino->mino = *this->holdMino->mino.original();

      this->currentMino = this->nextMino;
      this->nextMino = NULL;

      this->currentMino->x = this->holdMino->x;
      this->currentMino->y = this->holdMino->y;
    } else {
      Tetromino2D* tmp = this->currentMino;
      this->currentMino = this->holdMino;
      this->currentMino->x = tmp->x;
      this->currentMino->y = tmp->y;
      this->holdMino = tmp;
      this->holdMino->mino = *tmp->mino.original();
    }

    // this->tetrominos.erase(this->currentMino);
  }

  if (this->pressedKey == console::Key::K_UP) {
    // 하드 드롭
    this->currentMino->y += this->findRemainingDownwardDistance(
      this->currentMino->mino,
      this->currentMino->x,
      this->currentMino->y
    );
    this->currentMino = NULL;
    this->canUseHold = true;
  } else {
    // 소프트 드롭
    if (this->pressedKey == console::Key::K_X) {
      Tetromino rotatedMino = this->currentMino->mino.rotatedCW();
      
      // 돌릴수 있는지 확인
      CollisionType collisionType = this->collisionTester(
        rotatedMino, 
        this->currentMino->x, 
        this->currentMino->y, 
        &this->currentMino->mino
      );
      if (collisionType == CollisionType::NONE) {
        this->currentMino->mino = rotatedMino;
      }
    }

    if (this->pressedKey == console::Key::K_Z) {
      Tetromino rotatedMino = this->currentMino->mino.rotatedCCW();

      // 돌릴수 있는지 확인
      CollisionType collisionType = this->collisionTester(
        rotatedMino, 
        this->currentMino->x, 
        this->currentMino->y, 
        &this->currentMino->mino
      );
      if (collisionType == CollisionType::NONE) {
        this->currentMino->mino = rotatedMino;
      }
    }

    if (this->pressedKey == console::Key::K_DOWN) {
      speed = 10;
    }

    if ((tick % (int)std::round(DROP_DELAY / speed)) == 0) {
      y = 1;
    }

    CollisionType collisionType = this->collisionTester(this->currentMino->mino, this->currentMino->x + x, this->currentMino->y + y);  
    if (collisionType == CollisionType::NONE) {
      this->currentMino->x += x;
      this->currentMino->y += y;
    }

    if (collisionType == CollisionType::OUT_OF_BOARD_Y) {
      this->currentMino = NULL;
      this->canUseHold = true;
    }

    if (collisionType == CollisionType::CONFLICT_BLOCK) {
      if (y == 1) {
        this->currentMino = NULL;
        this->canUseHold = true;
      }
    }
  }

  this->pressedKey = console::Key::K_NONE;
}

Tetromino2D* Game::spawnTetromino(int x, int y) {
  int rd = getRandomTetrominoIndex();

  return new Tetromino2D {
    DECLARED_TETROMINOS[rd],
    x, y
  };
}

CollisionType Game::collisionTester(Tetromino& targetMino, int simulateX, int simulateY, Tetromino* excludeMino) {
  bool targetMinoArea[BOARD_HEIGHT][BOARD_WIDTH] = { { false, }, };

  for (int x = 0; x < targetMino.size(); ++x) {
    for (int y = 0; y < targetMino.size(); ++y) {
      if (targetMino.check(y, x)) {
        int absX = simulateX + x;
        int absY = simulateY + y;

        if (absX < 1 || absX >= BOARD_WIDTH) {
          return CollisionType::OUT_OF_BOARD_X;
        }

        if (absY >= BOARD_HEIGHT) {
          return CollisionType::OUT_OF_BOARD_Y;
        }

        targetMinoArea[absY][absX] = true;
      }
    }
  }

  for (Tetromino2D* item : this->tetrominos) {
    Tetromino& itemMino = item->mino;

    if ((&itemMino) == (&targetMino) || (&itemMino) == (excludeMino)) {
      continue;
    }

    for (int x = 0; x < itemMino.size(); ++x) {
      for (int y = 0; y < itemMino.size(); ++y) {
        if (itemMino.check(y, x)) {
          int absX = x + item->x;
          int absY = y + item->y;
          
          if (targetMinoArea[absY][absX] == true) {
            return CollisionType::CONFLICT_BLOCK;
          }
        }
      }
    }
  }

  return CollisionType::NONE;
}

int Game::findRemainingDownwardDistance(
  Tetromino& mino, 
  int simulateX, 
  int simulateY
) {
  int y = 0;
  while (this->collisionTester(this->currentMino->mino, this->currentMino->x, this->currentMino->y + (y + 1)) == CollisionType::NONE) {
    ++y;
  }

  return y;
}

bool Game::shouldExit() {
  return false;
}