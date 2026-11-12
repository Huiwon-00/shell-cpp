#include "builtin.h"
#include "redirect.h"
#include "pipeline.h"
#include "tokenizer.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::unordered_set<std::string> builtins = {"exit", "echo", "type", "pwd", "cd"};
  Pipeline pipeline;

  const char* path_env {std::getenv("PATH")};
  std::string filepath{path_env ? path_env : ""};
  std::vector<std::string> paths = split_path(filepath);
  std::string current_directory = std::filesystem::current_path().string();

  std::string command;

  while (true)
  {
    std::cout << "$ ";
    if (!std::getline(std::cin, command))
      break;
    std::vector<Token> tokens = tokenize(command);
    if ( tokens.empty() || !parse(tokens, pipeline) )
      continue;
    if(pipeline.size() == 1 && pipeline[0].args[0] == "exit"){
      break;
    }
    pipelines(pipeline, paths, builtins, current_directory);
    pipeline = {};
  }
}

