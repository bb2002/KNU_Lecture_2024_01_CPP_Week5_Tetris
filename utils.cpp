#include "utils.h"
#include <cstdlib>

int idx = 0;

int getRandomTetrominoIndex() {
  // return rand() % 7;
  int arr[] = { 6, 3, 4, 1, 5, 2, 1, 6, 0,0,0,0,0 };
  return arr[idx++];
}

std::string timeFormat(clock_t playTime) {
  double playTimeSeconds = (int)(playTime) / CLOCKS_PER_SEC;
  int minutes = (int)(playTimeSeconds / 60);
  int seconds = (int)(playTimeSeconds) % 60;
  int milliseconds = (int)((playTime % CLOCKS_PER_SEC) * 100 / CLOCKS_PER_SEC);

  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(2) << minutes << ":";
  oss << std::setfill('0') << std::setw(2) << seconds << ".";
  oss << std::setfill('0') << std::setw(2) << milliseconds << std::endl;

  return oss.str();
}