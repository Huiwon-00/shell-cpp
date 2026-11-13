#include "pipeline.h"
#include "builtin.h"
#include "redirect.h"
#include <iostream>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

void pipelines(Pipeline& pipeline, const std::vector<std::string>& paths,
    const std::unordered_set<std::string>& builtins, std::string& current_directory){
    std::vector<char*> arguments_c;
    std::vector<int> PIDs;
    int fds[2];
    int prevRead;
    for(size_t i = 0; i < pipeline.size(); i++){
        if(i != 0){
            prevRead = fds[0];
        }
        if(i != pipeline.size() - 1){
            if(pipe(fds) == -1){
                perror("pipe creation failed");
                return;
            }
        }
        pid_t pid = fork();
        if(pid == -1){
            perror("fork failed");
            return;
        }
        if(pid == 0){
            if(pipeline.size() == 1){
                //skip
            }
            else if(i == 0){
                if(dup2(fds[1], STDOUT_FILENO) == -1){
                    perror("dup2 failed to redirect");
                    std::exit(1);
                }
                close(fds[1]);
                close(fds[0]);
            }
            else if(i == pipeline.size() - 1){
                if(dup2(prevRead, STDIN_FILENO) == -1){
                    perror("dup2 failed to redirect");
                    std::exit(1);
                }
                close(prevRead);
            }
            else{
                if(dup2(prevRead, STDIN_FILENO) == -1){
                    perror("dup2 failed to redirect");
                    std::exit(1);
                }
                if(dup2(fds[1], STDOUT_FILENO) == -1){
                    perror("dup2 failed to redirect");
                    std::exit(1);
                }
                close(prevRead);
                close(fds[1]);
                close(fds[0]);
            }
            if(!builtins.contains(pipeline[i].args[0])){
                Command c = pipeline[i];    
                auto [stdout_fd, stderr_fd] = Redirect(c.stdout_file, c.stderr_file,c.append_stdout, c.append_stderr);
                if(stdout_fd == -2 || stderr_fd == -2)
                    std::exit(1);
                std::string fullpath = find_external_file_path(pipeline[i].args[0], paths);
                if(!fullpath.empty()){
                    for(std::string& s: pipeline[i].args){
                        arguments_c.push_back(s.data());
                    }
                    arguments_c.push_back(nullptr);
                    int execvErr {0};
                    execvErr = execv(fullpath.c_str(), arguments_c.data());
                    if(execvErr == -1){
                        perror(fullpath.c_str());
                    }
                }
                else{
                    std::cerr << pipeline[i].args[0] << ": command not found" << std::endl;
                }
            }
            else{
                Command c = pipeline[i];
                auto [stdout_fd, stderr_fd] = Redirect(c.stdout_file, c.stderr_file,c.append_stdout, c.append_stderr);
                if(stdout_fd == -2 || stderr_fd == -2)
                    std::exit(1);
                if (pipeline[i].args[0] == "echo")
                    echo(pipeline[i].args);
                else if (pipeline[i].args[0] == "pwd")
                    pwd(current_directory);
                else if (pipeline[i].args[0] == "cd")
                    cd(pipeline[i].args, current_directory);
                else
                    type(pipeline[i].args, paths, builtins);
            }
            std::exit(1);
        }
        if(i != pipeline.size() - 1){
            close(fds[1]);
        }
        PIDs.push_back(pid); 
        if(i != 0){
            close(prevRead);
        }
    }
    for(int p : PIDs){
        waitpid(p, nullptr, 0);
    }
    
}
