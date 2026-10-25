#include "tokenizer.h"

enum class QuoteStatus { None, Single, Double };

std::vector<Token> tokenize(const std::string& command){
  QuoteStatus quote = QuoteStatus::None;
  std::vector<Token> Tokens;
  Token t;  

  for(size_t i = 0; i < command.length(); i++){
    if(command[i] == '\\'){
      if(quote == QuoteStatus::None && i + 1 < command.length()){
        i++;
        t.text.push_back(command[i]);
      }
      else{
        if(i + 1 < command.length() && quote == QuoteStatus::Double &&
           (command[i + 1] == '\\'|| command[i + 1] == '"')){
          i++;
          t.text.push_back(command[i]);
        }
        else{
          t.text.push_back(command[i]);
        }
      }
    }

    else if(command[i] == '\''){
      if(quote == QuoteStatus::Single){
        quote = QuoteStatus::None;
      }
      else if(quote == QuoteStatus::Double){
        t.text.push_back(command[i]);
      }
      else{
        quote = QuoteStatus::Single;
      }
    }

    else if(command[i] == '"'){
      if(quote == QuoteStatus::Double){
        quote = QuoteStatus::None;
      }
      else if(quote == QuoteStatus::Single){
        t.text.push_back(command[i]);
      }
      else{
        quote = QuoteStatus::Double;
      }
    }

    else if(quote != QuoteStatus::None){
      t.text.push_back(command[i]);
    }

    else if(command[i] == ' '){
      if(!t.text.empty()){
        Tokens.push_back(t);
        t = Token{};      
      }
    }
    else if(command[i] == '|'){
      if(!t.text.empty()){
        Tokens.push_back(t);     
      }
      t.text = "|";
      t.type = TokenType::Pipe;
      Tokens.push_back(t);
      t = Token{};
    }

    else if(command[i] == '>'){
      bool err = t.text == "2";
      if(t.text == "1" || t.text == "2"){
        t = Token{};
      }
      if(!t.text.empty()){
        Tokens.push_back(t); 
      }
      if(i + 1 < command.length() && command[i + 1] == '>'){
        if(err){
          t.text = "2>>";
          t.type = TokenType::AppendErr;
        }
        else{
          t.text = ">>";
          t.type = TokenType::AppendOut;
        }
        i++;
      }
      else{
        if(err){
          t.text = "2>";
          t.type = TokenType::RedirectErr;
        }
        else{
          t.text = ">";
          t.type = TokenType::RedirectOut;
        }
      }
      Tokens.push_back(t);
      t = Token{};
    }

    else{
      t.text.push_back(command[i]);
    }
  }
  if(!t.text.empty()) Tokens.push_back(t);

  return Tokens;  
}
