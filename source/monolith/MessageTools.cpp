//
// Created by zirtoshka on 19.10.2024.
//

#include "MessageTools.hpp"

#include <string>

namespace monolith {

std::string getColorCode(Color color) {
  switch (color) {
    case Color::RED:
      return "\033[31m";
    case Color::YELLOW:
      return "\033[33m";
    case Color::GREEN:
      return "\033[32m";
    case Color::BLUE:
      return "\033[34m";
    case Color::MAGENTA:
      return "\033[35m";
    case Color::CYAN:
      return "\033[36m";
    case Color::RESET:
      return "\033[0m";
  }
  return "\033[0m";
}

static void printColoredMessage(const std::string& message, Color color, int hi) {
  if (hi != 0) {
    std::cout << getColorCode(color) << message << getColorCode(Color::RESET);
  } else {
    std::cout << getColorCode(color) << message << getColorCode(Color::RESET) << '\n';
  }
}

void PrintShellHi(const std::string& message) {
  printColoredMessage(message, Color::YELLOW, 1);
};
void PrintInfo(const std::string& message) {
  printColoredMessage(message, Color::BLUE, 0);
};
void PrintError(const std::string& message) {
  printColoredMessage(message, Color::RED, 0);
};

}  // namespace monolith