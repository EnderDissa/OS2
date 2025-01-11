#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>

namespace monolith {

std::string HelloWorld();
void ExecuteCommand(const std::vector<std::string>& args);
int ShellRun();
std::vector<std::string> ParseInput(const std::string& input);

}
