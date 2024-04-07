#include "game.h"

Game::Game() {
  srand(1); 
  this->currentMino = this->spawnTetromino(5, 1);
  srand(time(NULL));
  this->nextMino = this->spawnTetromino(13, 1);
  // 다음 테트리미노와 현재 테트리미노를 동시에 초기화하면 
  // 렌덤이 같은 값을 반환할 확률이 커져서
  // 시드를 다르게 준다.

  this->tetrominos[this->tetrominoSize++] = this->currentMino;
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
  if (this->nextMino != NULL) {
    this->nextMino->mino.drawAt(BLOCK_STRING, this->nextMino->x, this->nextMino->y);
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
  for (int i = 0; i < this->tetrominoSize; ++i) {
    Tetromino2D* mino2D = this->tetrominos[i];
    mino2D->mino.drawAt(BLOCK_STRING, mino2D->x, mino2D->y);
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

    this->tetrominos[this->tetrominoSize - 1] = this->currentMino;
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
      CollisionType collisionType = this->collisionTester(rotatedMino, this->currentMino->x, this->currentMino->y, &this->currentMino->mino);
      if (collisionType == CollisionType::NONE) {
        this->currentMino->mino = rotatedMino;
      }
    }

    if (this->pressedKey == console::Key::K_Z) {
      Tetromino rotatedMino = this->currentMino->mino.rotatedCCW();

      // 돌릴수 있는지 확인
      CollisionType collisionType = this->collisionTester(rotatedMino, this->currentMino->x, this->currentMino->y, &this->currentMino->mino);
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
  Tetromino2D* newTetromino = new Tetromino2D{
    Tetromino(
      TETROMINO_NAME[rd],
      TETROMINO_SIZE[rd],
      TETROMINO_SHAPE[rd]
    ), x,y
  };

  return newTetromino;
}

CollisionType Game::collisionTester(Tetromino& targetMino, int simulateX, int simulateY, Tetromino* excludeMino) {
  bool targetMinoArea[BOARD_WIDTH][BOARD_HEIGHT] = { { false, }, };
  for (int y = 0; y < targetMino.size(); ++y) {
    for (int x = 0; x < targetMino.size(); ++x ) {
      if (targetMino.check(x, y)) {
        int absoluteX = x + simulateX;
        int absoluteY = y + simulateY;

        if (absoluteX >= BOARD_WIDTH || absoluteX <= 0) {
          return CollisionType::OUT_OF_BOARD_X;
        }

        if (absoluteY >= BOARD_HEIGHT) {
          return CollisionType::OUT_OF_BOARD_Y;
        }

        targetMinoArea[absoluteX][absoluteY] = true;
      }
    }
  }

  for (int i = 0; i < this->tetrominoSize; ++i) {
    Tetromino2D* arrayedTetromino2D = this->tetrominos[i];
    Tetromino& arrayedMino = this->tetrominos[i]->mino;

    if ((&arrayedMino) == (&targetMino) || (&arrayedMino) == excludeMino) {
      continue;
    }

    for(int y = 0; y < arrayedMino.size(); ++y) {
      for(int x = 0; x < arrayedMino.size(); ++x) {
        if (arrayedMino.check(x, y)) {
          int absoluteX = x + arrayedTetromino2D->x;
          int absoluteY = y + arrayedTetromino2D->y;

          if (targetMinoArea[absoluteX][absoluteY] == true) {
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