#pragma once
#include <string>
#include <vector>

struct Token {
    std::string text;    
    bool quoted;         
};

enum class TokenType {
    Word,
    Pipe,
    RedirectOut,    // > or 1>
    RedirectErr,    // 2>
    AppendOut,      // >> or 1>>
    AppendErr,      // 2>>
};

std::vector<std::string> tokenize(const std::string& command, const char spliter);
