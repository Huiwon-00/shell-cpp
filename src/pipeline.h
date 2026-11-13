#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "command.h"

void pipelines( Pipeline& pipeline, const std::vector<std::string>& paths,
    const std::unordered_set<std::string>& builtins, std::string& current_directory);
