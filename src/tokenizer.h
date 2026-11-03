#pragma once
#include <string>
#include <vector>


enum class TokenType {
    Word,
    Pipe,
    RedirectOut,    // > or 1>
    RedirectErr,    // 2>
    AppendOut,      // >> or 1>>
    AppendErr,      // 2>>
};

struct Token {
    TokenType type = TokenType::Word;    
    std::string text;         
};

std::vector<Token> tokenize(const std::string& command);
std::vector<std::string> split_path(const std::string& file_path);
