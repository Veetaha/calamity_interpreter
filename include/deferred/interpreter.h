#pragma once

#include "tree.h"
#include "var.h"

namespace Iterpreter {
	Var execute(const Tree *astTree, const std::string & code);
	Var execute(const std::string & code);
}

