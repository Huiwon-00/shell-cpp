#include "redirect.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstdio>

bool fdHasError(const int& fd, const std::string& file){
    if(fd == -1){
        perror(file.c_str());
        return true;
    }
    return false;
}

std::pair<int, int> Redirect(const std::string& stdout_file, const std::string& stderr_file,
    const bool& append_stdout, const bool& append_stderr){
    return { stdout_file.empty() ? -1 :  R_stdout(stdout_file, append_stdout), // return -1 is the files are empty
            stderr_file.empty() ? -1 : R_stderr(stderr_file, append_stderr)}; //return -2 if there is a error with nonempty files
}

int R_stdout(const std::string& file, const bool& append_stdout ){
    int fd;
    if(append_stdout)
        fd = open(file.c_str(), O_CREAT | O_APPEND| O_WRONLY, 0644);
    else
        fd = open(file.c_str(), O_CREAT | O_TRUNC| O_WRONLY, 0644);
    if(fdHasError(fd, file))
        return -2;
    int restore_fd = dup(1);
    dup2(fd, 1);
    close(fd);
    return restore_fd;
}
int R_stderr(const std::string& file, const bool& append_stderr){
    int fd;
    if(append_stderr)
        fd = open(file.c_str(), O_CREAT | O_APPEND| O_WRONLY, 0644);
    else
        fd = open(file.c_str(), O_CREAT | O_TRUNC| O_WRONLY, 0644);
    if(fdHasError(fd, file))
        return -2;
    int restore_fd = dup(2);
    dup2(fd, 2);
    close(fd);
    return restore_fd;
}

void restore(const int& stdout_restore_fd, const int& stderr_restore_fd){
    if(stdout_restore_fd != -1){
        dup2(stdout_restore_fd, 1);
        close(stdout_restore_fd);
    }
    if(stderr_restore_fd != -1){
        dup2(stderr_restore_fd, 2);
        close(stderr_restore_fd);
    }
}




