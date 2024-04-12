#include "game.h"

Game::Game() {
  srand(time(NULL) + 1);
  this->currentMino = this->spawnTetromino(5,3);
  this->tetrominos.push_back(this->currentMino);

  // 다음 테트리미노와 현재 테트리미노를 동시에 초기화하면 
  // 렌덤이 같은 값을 반환할 확률이 커져서
  // 시드를 다르게 준다.
  srand(time(NULL));
  this->nextMino = this->spawnTetromino(13, 2);
}

void Game::update() {
  ++tick;
  if (this->status == GameStatus::PLAYING) {
    this->playTime = clock() - this->startAt;

    if (this->nextMino == NULL) {
      this->nextMino = this->spawnTetromino(13, 2);
    }

    int completedLine = this->detectListCompleted();
    if (completedLine != -1) {
      this->removeLineAndGetPoint(completedLine);
    } 

    if (this->score >= LINES) {
      this->status = GameStatus::WIN;
      return;
    }

    this->inputListener();
    this->moveTetromino(tick);

    if (tick == 1 || tick % DROP_DELAY == 0) {
      this->updateSlowly();
      return;
    }
  }

  if (this->status == GameStatus::EXIT) {
    exit(0);
  }
}

void Game::updateSlowly() {
  if (this->currentMino == NULL) {
    this->currentMino = this->nextMino;
    this->nextMino = NULL;
    this->currentMino->x = 5;
    this->currentMino->y = 1;

    CollisionType collision = this->collisionTester(
      *this->currentMino->mino, 
      this->currentMino->x, 
      this->currentMino->y);

    if (collision == CollisionType::CONFLICT_BLOCK) {
      // 스폰하자마자 충돌한 경우
      status = GameStatus::LOSE;
      return;
    }

    this->tetrominos.push_back(this->currentMino);
  }
}

void Game::draw() {
  this->drawTetrominos();
  this->drawUI();
}

void Game::drawUI() {
  // 화면 생성
  console::drawBox(0, 0, BOARD_WIDTH + 1,BOARD_HEIGHT + 1);
  
  // Next 상자 생성
  console::drawBox(12, 0, 12 + TETRIS_UI_WIDTH, TETRIS_UI_HEIGHT);
  console::draw(13, 0, "Next");

  // Hold 상자 생성
  console::drawBox(18, 0, 18 + TETRIS_UI_WIDTH, TETRIS_UI_HEIGHT);
  console::draw(19, 0, "Hold");

  // 게임 진행상황 표시기
  console::draw(0, BOARD_HEIGHT + 2, std::to_string(LINES - this->score) + " lines left");
  console::draw(2, BOARD_HEIGHT + 3, timeFormat(this->playTime));

  // 승리/패배 표시
  if (this->status == GameStatus::WIN) {
    console::draw(3, 9, "You Win");
    console::draw(2, 10, timeFormat(this->playTime));
  }
  
  if (this->status == GameStatus::LOSE) {
    console::draw(2, 9, "You Lost");
  }
}

void Game::drawTetrominos() {
  if (this->nextMino != NULL) {
    this->nextMino->mino->drawAt(BLOCK_STRING, this->nextMino->x, this->nextMino->y);
  }

  if (this->currentMino != NULL) {
    this->currentMino->mino->drawAt(
      SHADOW_STRING,
      this->currentMino->x,
      this->currentMino->y + this->findRemainingDownwardDistance(
        *this->currentMino->mino,
        this->currentMino->x,
        this->currentMino->y
      )
    );
  }

  if (this->holdMino != NULL) {
    this->holdMino->mino->drawAt(
      BLOCK_STRING,
      20, 2
    );
  }

  // Draw Tetrominos
  for (Tetromino2D* item : this->tetrominos) {
    item->mino->drawAt(BLOCK_STRING, item->x, item->y);
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
  if (this->pressedKey == console::Key::K_ESC) {
    this->status = GameStatus::EXIT;
    return;
  }

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
    for (int i = 0; i < this->tetrominos.size(); ++i) {
      if (this->currentMino == this->tetrominos[i]) {
        this->tetrominos.erase(this->tetrominos.begin() + i);
      }
    }

    if (this->holdMino == NULL) {
      this->holdMino = this->currentMino;

      // 스파게티 코드의 최후
      // 이 주석을 보실지 모르겠으나, 회전한 도형을 current->mino에 할당하는 순간
      // original 까지 돌아간 데이터로 덮어씌워지는 기히한 현상으로 인해
      // (아마 copy  관련 문제 같습니다)
      // 이를 피하고자 돌릴때마다 새로운 테트리미노를 생성해서...
      // 이런 이상한 코드가 나왔다고 합니다.
      // 비록 메모리를 조금 쓰지만 잘 동작합니다
      Tetromino* before = this->holdMino->mino->original();
      while (true) {
        if (before->original() == before) {
          this->holdMino->mino = before;
          break;
        }

        before = before->original();
      }
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
      
      Tetromino* before = this->holdMino->mino->original();
      while (true) {
        if (before->original() == before) {
          this->holdMino->mino = before;
          break;
        }

        before = before->original();
      }
    }

    // 렌더 배열에 다시 추가
    this->tetrominos.push_back(this->currentMino);
  }

  if (this->pressedKey == console::Key::K_UP) {
    // 하드 드롭
    this->currentMino->y += this->findRemainingDownwardDistance(
      *this->currentMino->mino,
      this->currentMino->x,
      this->currentMino->y
    );
    this->currentMino = NULL;
    this->canUseHold = true;
  } else {
    // 소프트 드롭
    if (this->pressedKey == console::Key::K_X) {
      // original 도형까지 돌아가버리는 이상한 현상으로 인해
      // 이상한 코드 탄생
      Tetromino* rotatedMino = new Tetromino(this->currentMino->mino->rotatedCW());

      // 돌릴수 있는지 확인
      CollisionType collisionType = this->collisionTester(
        *rotatedMino, 
        this->currentMino->x, 
        this->currentMino->y, 
        this->currentMino->mino
      );
      if (collisionType == CollisionType::NONE) {
        this->currentMino->mino = rotatedMino;
      }
    }

    if (this->pressedKey == console::Key::K_Z) {
      // original 도형까지 돌아가버리는 이상한 현상으로 인해
      // 이상한 코드 탄생
      Tetromino* rotatedMino = new Tetromino(this->currentMino->mino->rotatedCCW());

      // 돌릴수 있는지 확인
      CollisionType collisionType = this->collisionTester(
        *rotatedMino, 
        this->currentMino->x, 
        this->currentMino->y, 
        this->currentMino->mino
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

    CollisionType collisionType = this->collisionTester(*this->currentMino->mino, this->currentMino->x + x, this->currentMino->y + y);  
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
  Tetromino* newMino = new Tetromino(DECLARED_TETROMINOS[rd]);

  return new Tetromino2D {
    newMino,
    x, y
  };
}

CollisionType Game::collisionTester(Tetromino& targetMino, int simulateX, int simulateY, Tetromino* excludeMino) {
  bool targetMinoArea[BOARD_HEIGHT + 1][BOARD_WIDTH + 1] = { { false, }, };

  for (int x = 0; x < targetMino.size(); ++x) {
    for (int y = 0; y < targetMino.size(); ++y) {
      if (targetMino.check(y, x)) {
        int absX = simulateX + x;
        int absY = simulateY + y;

        if (absX < 1 || absX >= (BOARD_WIDTH+1)) {
          return CollisionType::OUT_OF_BOARD_X;
        }

        if (absY >= (BOARD_HEIGHT+1)) {
          return CollisionType::OUT_OF_BOARD_Y;
        }

        targetMinoArea[absY][absX] = true;
      }
    }
  }

  for (Tetromino2D* item : this->tetrominos) {
    Tetromino* itemMino = item->mino;

    if ((itemMino) == (&targetMino) || (itemMino) == (excludeMino)) {
      continue;
    }

    for (int x = 0; x < itemMino->size(); ++x) {
      for (int y = 0; y < itemMino->size(); ++y) {
        if (itemMino->check(y, x)) {
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
  while (this->collisionTester(*this->currentMino->mino, this->currentMino->x, this->currentMino->y + (y + 1)) == CollisionType::NONE) {
    ++y;
  }

  return y;
}

int Game::detectListCompleted() {
  bool currentBoard[BOARD_HEIGHT + 1][BOARD_WIDTH + 1] = { {false,}, };
  
  for (Tetromino2D* mino : this->tetrominos) {
    Tetromino* tetromino = mino->mino;

    if (mino == this->currentMino) {
      // 아직 내려가지 않은 테트리미노는 스킵
      continue;
    }

    for (int x = 0; x < tetromino->size(); ++x) {
      for (int y = 0; y < tetromino->size(); ++y) {
        if (tetromino->check(y, x)) {
          int absX = x + mino->x;
          int absY = y + mino->y;
          currentBoard[absY][absX] = true;
        }
      }
    }
  }

  for (int i = 0; i < BOARD_HEIGHT + 1; ++i) {
    bool isFull = false;
    for (int j = 1; j < BOARD_WIDTH + 1; ++j) {
      isFull = currentBoard[i][j];
      if (!isFull) {
        break;
      }
    }

    if (isFull) {
      return i;
    }
  }

  return -1;
}

void Game::removeLineAndGetPoint(int line) {
  std::vector<Tetromino2D*> targetMinos;

  for (Tetromino2D* mino : this->tetrominos) {
    Tetromino* tetromino = mino->mino;
    const int sz = tetromino->size();

    // 삭제 될 라인에 해당하는 테트리미노 찾기
    for (int x = 0; x < tetromino->size(); ++x) {
      for (int y = 0; y < tetromino->size(); ++y) {
        if (tetromino->check(y, x)) {
          int absY = y + mino->y;

          if (absY == line) {
            bool insert = true;
            for (int i = 0; i < targetMinos.size(); ++i) {
              if (targetMinos[i] == mino) {
                insert = false;
                break;
              }
            }
            if (insert) {
              targetMinos.push_back(mino);
            }
          }
        }
      }
    }
  }

  for (Tetromino2D* mino : targetMinos) {
    Tetromino* tetromino = mino->mino;

    // shape 에 직접 접근이 안되니 check() 하여 리버스
    std::vector<std::vector<bool>> reversedShape(
      tetromino->size(),
      std::vector<bool>(tetromino->size(), false)
    );

    std::vector<std::vector<bool>> lineRemovedShape(
      tetromino->size(),
      std::vector<bool>(tetromino->size(), false)
    );

    for (int x = 0; x < tetromino->size(); ++x) {
      for (int y = 0; y < tetromino->size(); ++y) {
        reversedShape[x][y] = tetromino->check(x, y);
      }
    }

    for (int y = 0; y < reversedShape.size(); ++y) {
      for (int x = 0; x < reversedShape.size(); ++x) {
        int absY = y + mino->y;
        if (absY != line) {
          if (absY < line) {
            lineRemovedShape[y + 1][x] = reversedShape[y][x];
          } else {
            lineRemovedShape[y][x] = reversedShape[y][x];
          }
        }
      }
    }
    

    // 문자열로 직렬화
    std::string serialized = "";
    for (int y = 0; y < reversedShape.size(); ++y) {
      for (int x = 0; x < reversedShape.size(); ++x) {
        serialized += lineRemovedShape[y][x] ? "O" : "X";
      }
    }

    Tetromino* resultShape = new Tetromino(
      tetromino->name(),
      tetromino->size(),
      serialized
    );

    // 기존 테트리미노 삭제
    delete mino->mino;

    // 렌더 배열에 반영
    mino->mino = resultShape;
  }
  
  // targetMinos 가 아닌 애들은 절대 좌표를 하나씩 내리기
  for (Tetromino2D* tetromino : this->tetrominos) {
    bool find = false;
    for (Tetromino2D* targetMino : targetMinos) {
      if (targetMino == tetromino) {
        find = true;
      }
    }

    if (!find) {
      Tetromino* mino = tetromino->mino;
      int floor = 0;

      for (int y = 0; y < mino->size(); ++y) {
        for (int x = 0; x < mino->size(); ++x) {
          int absY = y + tetromino->y;
          if (floor < absY) {
            floor = absY;
          }
        }
      }

      if (floor < line) {
        tetromino->y++;
      }
    }
  }

  this->score++;
}

bool Game::shouldExit() {
  return false;
}