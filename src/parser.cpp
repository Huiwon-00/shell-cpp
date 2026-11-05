#include "parser.h"
#include <vector>
#include <iostream>

bool parse(std::vector<Token>& tokens, Pipeline& out){
    Command command;

    for(size_t i = 0; i < tokens.size(); i ++){
        switch(tokens[i].type){
            case TokenType::Word:
                command.args.push_back(tokens[i].text);
                // add to current command's args

                break;
            case TokenType::Pipe:
                if(!command.args.empty()){
                    out.push_back(command);
                    command = Command{};
                }
                else{
                    std::cerr << "syntax error near unexpected token `|'\n";
                    return false;
                }
                break;
            case TokenType::RedirectOut:
                if(i + 1 < tokens.size()){
                    if(tokens[i + 1].type == TokenType::Word){
                        command.stdout_file = tokens[i + 1].text;
                        out.push_back(command);
                        command = Command{};
                        i++;
                    }
                    else{
                        std::cerr << "syntax error near unexpected token `" << tokens[i + 1].text << "'\n";
                        return false;
                    }
                }
                else{
                    std::cerr << "syntax error near unexpected token `newline'\n";
                    return false;
                }
                break;
            case TokenType::RedirectErr:
                if(i + 1 < tokens.size()){
                    if(tokens[i + 1].type == TokenType::Word){
                        command.stderr_file = tokens[i + 1].text;
                        out.push_back(command);
                        command = Command{};
                        i++;
                    }
                    else{
                        std::cerr << "syntax error near unexpected token `" << tokens[i + 1].text << "'\n";
                        return false;
                    }
                }
                else{
                    std::cerr << "syntax error near unexpected token `newline'\n";
                    return false;
                }
                break;
            case TokenType::AppendOut:
                if(i + 1 < tokens.size()){
                    if(tokens[i + 1].type == TokenType::Word){
                        command.stdout_file = tokens[i + 1].text;
                        command.append_stdout = true;
                        out.push_back(command);
                        command = Command{};
                        i++;
                    }
                    else{
                        std::cerr << "syntax error near unexpected token `" << tokens[i + 1].text << "'\n";
                        return false;
                    }
                }
                else{
                    std::cerr << "syntax error near unexpected token `newline'\n";
                    return false;
                }
                break;
            case TokenType::AppendErr:
                if(i + 1 < tokens.size()){
                    if(tokens[i + 1].type == TokenType::Word){
                        command.stderr_file = tokens[i + 1].text;
                        command.append_stderr = true;
                        out.push_back(command);
                        command = Command{};
                        i++;
                    }
                    else{
                        std::cerr << "syntax error near unexpected token `" << tokens[i + 1].text << "'\n";
                        return false;
                    }
                }
                else{
                    std::cerr << "syntax error near unexpected token `newline'\n";
                    return false;
                }
                break;
        }
    }
}
