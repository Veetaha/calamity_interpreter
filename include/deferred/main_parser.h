#pragma once 
#include <stdbool.h>
#include "tree.h"
#include "ast.h"
#include "token.h"

namespace Parser {

	Ast * buildNewAst(const std::vector<Token> *tokens, const std::string & code);

}