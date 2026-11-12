#pragma once
#include <string>
#include <vector>

struct Command{
    std::vector<std::string> args;
    std::string stdout_file;
    std::string stderr_file;
    bool append_stdout = false;
    bool append_stderr = false;
};

using Pipeline = std::vector<Command>;
