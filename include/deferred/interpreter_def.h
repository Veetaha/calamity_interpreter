#pragma once 

#include <dict.h>
#include <sstring.h>
#include <ast.h>
#include <var.h>

enum ProgState {
    ProgState_IDLE,
    ProgState_RETURN,
    ProgState_ESCAPE,
    ProgState_ERROR
};


typedef struct __Calamity{

              Dict * vars;
              Dict * funcs;
              Dict * defs;
            String * error;
              List * locals;
       Var * status;
    enum ProgState   state;
    const   String * code;
} Calamity;

extern Var * evaluate_new(Calamity * prog, Tree * ast);

void Calamity_setError(const char * msg, AstNode * node, Calamity * prog);

void MACRO_FREE_DYNTYPE_ARR(Var ** arr, size_t size);

#define __CHECK_EXPANSION(retval, expr, ...){\
  if (!(expr)) {                                            \
      Var * ___freeArr[] = {__VA_ARGS__};           \
      MACRO_FREE_DYNTYPE_ARR(___freeArr,                    \
                          sizeof(___freeArr)                \
                          / sizeof(Var *));         \
      return retval;                \
  }                                                         \
}

// ISO C99 requires at least one argument for the "..." in a variadic macro
#define __CHECK(retval, ...) __CHECK_EXPANSION(retval, __VA_ARGS__, NULL)


#define __EXPECT_EXPANSION(retval, expr, errorMsg, errTreeNode, ...){                   \
    if (!(expr)) {                                                                      \
        const Tree * ___errTree = errTreeNode;                                          \
        while (AstNode_locationAt(Tree_at(___errTree)).x == 0){                         \
            if (!Tree_isLeaf(___errTree)){                                              \
              ___errTree = Tree_childAt(___errTree, 0);                                 \
            } else {                                                                    \
                Debug_shutdown("Failed to find terminal ast node to write error about");\
            }                                                                           \
        }                                                                               \
        Calamity_setError(errorMsg, Tree_at(___errTree), prog);                            \
        Var * ___freeArr[] = {__VA_ARGS__};                                     \
        MACRO_FREE_DYNTYPE_ARR(___freeArr,                                              \
                            sizeof(___freeArr)                                          \
                            / sizeof(Var *));                                   \
        return retval;                                                                  \
    }                                                                                   \
}

// ISO C99 requires at least one argument for the "..." in a variadic macro
#define __EXPECT(retval, expr, errorMsg, ...) __EXPECT_EXPANSION(retval, expr, errorMsg, __VA_ARGS__, NULL)
