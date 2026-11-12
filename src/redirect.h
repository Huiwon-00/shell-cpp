#pragma once
#include <string>
#include <utility>

bool fdHasError(const int& fd, const std::string& file);
std::pair<int, int> Redirect(const std::string& stdout_file, const std::string& stderr_file,
    const bool& append_stdout, const bool& append_stderr);
int R_stdout(const std::string& file, const bool& append_stdout );
int R_stderr(const std::string& file, const bool& append_stderr);
void restore(const int& stdout_restore_fd, const int& stderr_restore_fd);
