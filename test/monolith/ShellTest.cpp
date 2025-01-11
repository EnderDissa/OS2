#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "../../source/monolith/Shell.hpp"

// Custom assertion function
void Assert(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error("Assertion failed: " + message);
    }
}

// Helper function to capture output
std::string CaptureOutput(const std::string& command) {
    std::ostringstream oss;
    std::ostringstream err_oss;

    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
    std::streambuf* old_cerr = std::cerr.rdbuf(err_oss.rdbuf());

    std::istringstream input_stream(command + "\nexit\n");
    std::streambuf* old_cin = std::cin.rdbuf(input_stream.rdbuf());

    monolith::ShellRun();

    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);
    std::cerr.rdbuf(old_cerr);

    return err_oss.str() + oss.str();
}

// Test: Empty command error
void TestEmptyCommandError() {
    std::string input = "";
    std::string output = CaptureOutput(input);
    Assert(output.find("Emptiness is not a command") != std::string::npos,
           "Expected error message for empty command");
}

// Test: Execution time measurement
void TestExecutionTimeMeasurement() {
    std::string output = CaptureOutput("sleep 1");
    Assert(output.find("Program executed in") != std::string::npos,
           "Expected execution time message");
}

// Register all tests
void RunAllTests() {
    TestEmptyCommandError();
    TestExecutionTimeMeasurement();
    std::cout << "All tests passed!" << std::endl;
}
