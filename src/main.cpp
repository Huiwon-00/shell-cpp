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

    if(pipeline.size() == 1){
      if(builtins.contains(pipeline[0].args[0])){
        Command c = pipeline[0];
        auto [stdout_fd, stderr_fd] = Redirect(c.stdout_file, c.stderr_file,c.append_stdout, c.append_stderr);
        if(stdout_fd == -2 || stderr_fd == -2){
          restore(stdout_fd, stderr_fd);
          pipeline = {};
          continue;
        }
        const std::string name = pipeline[0].args[0];
        if (name == "echo")
            echo(pipeline[0].args);
        else if (name == "pwd")
            pwd(current_directory);
        else if (name == "cd")
            cd(pipeline[0].args, current_directory);
        else if(name == "type")
            type(pipeline[0].args, paths, builtins);
        restore(stdout_fd, stderr_fd);
        if(name == "exit")
          break;
      }
      else{
        executable(pipeline[0], paths);
      }
    }
    else{
      pipelines(pipeline, paths, builtins, current_directory);
    }
    pipeline = {};
  }
}

