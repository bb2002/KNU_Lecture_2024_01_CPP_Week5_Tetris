#include "utils.h"
#include <cstdlib>
#include <ctime>

int getRandomTetrominoIndex() {
  srand(time(NULL));
  return rand() % 7;
}