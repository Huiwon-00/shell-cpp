#pragma once
#include "command.h"
#include <string>
#include <vector>
#include <unordered_set>


void echo(const std::vector<std::string>& arguments);
void pwd(const std::string& current_directory);
void cd(const std::vector<std::string>& arguments, std::string& current_directory);
void type(const std::vector<std::string>& arguments, const std::vector<std::string>& paths,
        const std::unordered_set<std::string>& builtins);
void executable(const Command& c, const std::vector<std::string>& paths);
std::string find_external_file_path(const std::string& cmd, const std::vector<std::string>& paths);
