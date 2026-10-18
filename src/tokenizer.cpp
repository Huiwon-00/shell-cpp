#include "tokenizer.h"

std::vector<Token> tokenize(const std::string& command, const char spliter){
  int QuoteStatus = 0;
  std::vector<Token> Tokens;
  Token t;

  for(size_t i = 0; i < command.length(); i++){
    if(command[i] == '\\'){
      if(QuoteStatus == 0 && i + 1 < command.length()){
        i++;
        t.text.push_back(command[i]);
      }
      else{
        if((command[i + 1] == '\\'|| command[i + 1] == '"') && QuoteStatus == 2){
          i++;
          t.text.push_back(command[i]);
        }
        else{
          t.text.push_back(command[i]);
        }
      }
    }
    else if(command[i] == '\''){
      if(QuoteStatus == 1){
        QuoteStatus = 0;
      }
      else if(QuoteStatus == 2){
        t.text.push_back(command[i]);
      }
      else{
        QuoteStatus = 1;
      }
    }
    else if(command[i] == '"'){
      if(QuoteStatus == 2){
        QuoteStatus = 0;
      }
      else if(QuoteStatus == 1){
        t.text.push_back(command[i]);
      }
      else{
        QuoteStatus = 2;
      }
    }
    else if(QuoteStatus == 1 || QuoteStatus == 2){
      t.text.push_back(command[i]);
    }
    else if(command[i] == spliter){
      if(!t.text.empty()){
        Tokens.push_back(t);
        t.text.clear();
      }
    }
    else{
      t.text.push_back(command[i]);
    }
  }
  if(!t.text.empty()) Tokens.push_back(t);

  return Tokens;
}
