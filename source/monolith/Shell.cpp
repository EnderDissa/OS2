#include "Shell.hpp"

#include <sys/mman.h>
#include <sstream>

#include "MessageTools.hpp"

namespace monolith {
std::vector<std::string> ParseInput(const std::string& input) {
  std::vector<std::string> tokens;
  std::string item;
  std::stringstream ss(input);
  while (ss >> item) {
    tokens.push_back(item);
  }
  return tokens;
}

void ExecuteCommand(const std::vector<std::string>& args) {
  if (args.empty()) {
    PrintError("input is empty!");
    return;
  }

  std::vector<char*> argv;
  for (const auto& arg : args) {
    argv.push_back(const_cast<char*>(arg.c_str()));
  }
  argv.push_back(nullptr);

  auto start = std::chrono::high_resolution_clock::now();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  pid_t pid = vfork();
#pragma clang diagnostic pop

  if (pid < 0) {
    PrintError("vfork FAIL");
    return;
  }

  if (pid == 0) {
    execvp(argv[0], argv.data());
    PrintError("unknown FAIL");
    _exit(1);
  }

  int status;
  waitpid(pid, &status, 0);

  std::chrono::time_point end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  PrintInfo("Program executed in  " + std::to_string(elapsed.count()) + " seconds.");
}

int ShellRun() {
  std::string input;

  while (true) {
    PrintShellHi("shell--> ");

    std::getline(std::cin, input);

    if (input == "exit") {
      break;
    }

    std::vector<std::string> args = ParseInput(input);

    ExecuteCommand(args);
  }
  return 0;
}

}  // namespace monolith