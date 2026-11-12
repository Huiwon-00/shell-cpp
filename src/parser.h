#pragma once
#include <vector>
#include "command.h"
#include "tokenizer.h"

bool parse(std::vector<Token>& tokens, Pipeline& out);
