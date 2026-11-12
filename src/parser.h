#pragma once
#include <vector>
#include "tokenizer.h"
#include "builtin.h"
#include "pipeline.h"
#include "redirect.h"

struct Command{
    std::vector<std::string> args;
    std::string stdout_file;
    std::string stderr_file;
    bool append_stdout = false;
    bool append_stderr = false;
};

using Pipeline = std::vector<Command>;

bool parse(std::vector<Token>& tokens, Pipeline& out);