#include <parser.h>
#include <token.h>
#include <wchar.h>
#include <iterator.h>
#include <sstring.h>

#define NO_DEBUG_TRACE

#include <error.h>
#include <cui.h>

typedef struct __Parser{
    Iterator * iter;
    String * error;
    const String * code;
} Parser;

typedef Tree * (*GrammarRule)(Parser *);

static void Private_printDebug(Parser * parser){
    if (Iterator_inbound(parser->iter)){
        Token_print(Iterator_value(parser->iter));
    } else {
        printf("<NO TOKEN>");
    }     
    putchar('\n');
}
#ifndef NO_DEBUG_TRACE
#define PARSER_TRACE_CALL()                                      \
    TRACE_CALL(); /*<- all magic is here*/                       \
    Cui_printHLine(75 - _____getLevel() - strlen(__func__), ' ');\
    Private_printDebug(parser);                                  \
//  ^^^ globals and goto are awesome ^^^

#define PARSER_EXPECTED_TRACE_CALL() \
    TRACE_CALL();     \
    Cui_printHLine(75 - _____getLevel() - strlen(__func__), ' ');\
    fputs("EXPECTED: ", stdout);                                 \
    LexemeType_print(tokenType);                                 \
    fputs(" FACED: ", stdout);                                   \
    Private_printDebug(parser);                                  
#else 
    #define PARSER_TRACE_CALL()
    #define PARSER_EXPECTED_TRACE_CALL()
#endif


#define CHECK_EXPANSION(expr, ...){                                        \
    if (!(expr)) {                                                         \
        Tree * ___tree[] = {__VA_ARGS__};                                  \
        MACRO_FREE_TREE_ARR((___tree), sizeof(___tree) / sizeof(Tree *));  \
        return NULL;                                                       \
    }                                                                      \
}

// ISO C99 requires at least one argument for the "..." in a variadic macro
// checks first arg (condtion), if it is false frees the optional tree as the second arg
#define CHECK(...) CHECK_EXPANSION(__VA_ARGS__, NULL)                                 
                                
static void MACRO_FREE_TREE_ARR(Tree ** arr, size_t size){
    Debug_exists(arr);
    for (unsigned int i = 0; i < size - 1; ++i){
        if (arr[i]) Tree_freeWhole(arr[i], (Destructor)AstNode_freeWhole);
    }
}

// checks first arg (condtion), if it is false checks for errors and if
// error was deteced, frees tree at second argument
// otherwise adds tree at fourth arg as the last child for tree at the thirs arg
#define OPTIONAL(expr, tree, rootToAdd, optionalChild){                \
    if (!(expr) && parser->error) {                                    \
        Tree * ___tree = (tree);                                       \
        if ((___tree)){                                                \
            Tree_freeWhole((___tree), (Destructor)AstNode_freeWhole);  \
        }                                                              \
        return NULL;                                                   \
    } else {                                                           \
        Tree * ___root     = (rootToAdd);                              \
        Tree * ___optional = (optionalChild);                          \
        if (___optional && ___root)                                    \
            Tree_addLastChild(___root, ___optional);                   \
    }                                                                  \
}

#define EXPECT_RULE(expr, tree, errorMsg) {                                \
    if (!(expr)){                                                                    \
        Tree * ___tree = (tree);                                                     \
        Private_writeErrorRule(errorMsg, parser);                                \
        if ((___tree))                                                               \
            Tree_freeWhole((___tree), (Destructor)AstNode_freeWhole);                \
        return NULL;                                                                 \
    }                                                                                \
}
// very love macros
#define Tree_newAstConstruct(AST_NODE_TYPE) Tree_new(AstNode_new((AST_NODE_TYPE), NULL, loc(0,0), NULL))

#define Ebnf_multiple(parser, root, rule)  __Ebnf_multiple((parser), (root), ((GrammarRule)rule))

#define Ebnf_select(parser, ...)             __Ebnf_select((parser),                     \
                                                          ((GrammarRule[])__VA_ARGS__),  \
                                                   sizeof(((GrammarRule[])__VA_ARGS__)) /\
                                                   sizeof(((GrammarRule[])__VA_ARGS__[0])))
                                                              
#define Ebnf_selectLexeme(parser, ...) __Ebnf_selectLexeme((parser),                      \
                                                          ((LexemeType[])__VA_ARGS__),     \
                                                   sizeof(((LexemeType[])__VA_ARGS__)) /   \
                                                   sizeof(((LexemeType[])__VA_ARGS__[0])))
                                                              
#define Ebnf_apRule(parser, higherRule, ...) __Ebnf_apRule((parser),                     \
                                                           (GrammarRule)higherRule,      \
                                                          ((LexemeType[])__VA_ARGS__),    \
                                                   sizeof(((LexemeType[])__VA_ARGS__))  / \
                                                   sizeof(((LexemeType[])__VA_ARGS__[0])))


static AstNode * Token_toNewAstNode(Token * self){
    Debug_exists(self);
    //
    switch (self->lex->type){
        case LexemeType_AND:         return AstNode_new(AstNodeType_AND         , NULL, self->loc, NULL);    
        case LexemeType_EQ:          return AstNode_new(AstNodeType_EQ          , NULL, self->loc, NULL);    
        case LexemeType_GEQ:         return AstNode_new(AstNodeType_GEQ         , NULL, self->loc, NULL);    
        case LexemeType_GREATER:     return AstNode_new(AstNodeType_GREATER     , NULL, self->loc, NULL);        
        case LexemeType_LEQ:         return AstNode_new(AstNodeType_LEQ         , NULL, self->loc, NULL);    
        case LexemeType_LESS:        return AstNode_new(AstNodeType_LESS        , NULL, self->loc, NULL);    
        case LexemeType_NEQ:         return AstNode_new(AstNodeType_NEQ         , NULL, self->loc, NULL);    
        case LexemeType_NOT:         return AstNode_new(AstNodeType_NOT         , NULL, self->loc, NULL);    
        case LexemeType_OR:          return AstNode_new(AstNodeType_OR          , NULL, self->loc, NULL);    
        case LexemeType_ASSIGN:      return AstNode_new(AstNodeType_ASSIGN      , NULL, self->loc, NULL);        
        case LexemeType_APPEND:      return AstNode_new(AstNodeType_APPEND      , NULL, self->loc, NULL);        
        case LexemeType_CONCATENATE: return AstNode_new(AstNodeType_CONCATENATE , NULL, self->loc, NULL);            
        case LexemeType_CASE:        return AstNode_new(AstNodeType_CASE        , NULL, self->loc, NULL);    
        case LexemeType_WHILE:       return AstNode_new(AstNodeType_WHILE       , NULL, self->loc, NULL);    
        case LexemeType_DIV:         return AstNode_new(AstNodeType_DIV         , NULL, self->loc, NULL);    
        case LexemeType_MINUS:       return AstNode_new(AstNodeType_MINUS       , NULL, self->loc, NULL);    
        case LexemeType_MULT:        return AstNode_new(AstNodeType_MULT        , NULL, self->loc, NULL);    
        case LexemeType_PLUS:        return AstNode_new(AstNodeType_PLUS        , NULL, self->loc, NULL);    
        case LexemeType_POW:         return AstNode_new(AstNodeType_POW         , NULL, self->loc, NULL);    
        case LexemeType_REMAINDER:   return AstNode_new(AstNodeType_REMAINDER   , NULL, self->loc, NULL);        
        case LexemeType_ENDL: {
            DynamicType * val = DynamicType_new(Type_CHARACTER);
            DynamicType_setChar(val, '\n');
            return AstNode_new(AstNodeType_LITERAL, val, self->loc, NULL);   
        }
        case LexemeType_FALSE: {
            DynamicType * val = DynamicType_new(Type_BOOLEAN);
            DynamicType_setBool(val, false);
            return AstNode_new(AstNodeType_LITERAL, val, self->loc, NULL);   
        }
        case LexemeType_TRUE: {
            DynamicType * val = DynamicType_new(Type_BOOLEAN);
            DynamicType_setBool(val, true);
            return AstNode_new(AstNodeType_LITERAL, val, self->loc, NULL);   
        }
        case LexemeType_UNDEFINED:     return AstNode_new(AstNodeType_LITERAL, 
                                                         DynamicType_new(Type_UNDEFINED), self->loc, NULL);        
        case LexemeType_IDENTIFIER: {
            AstNode * node = AstNode_new(
                AstNodeType_IDENTIFIER, 
                NULL, 
                self->loc,
                String_newCopyOf(self->lex->name)
            );

            return node;
        }
        case LexemeType_STRING: {
            DynamicType * value = DynamicType_new(Type_STRING);
            String_concatenate(DynamicType_stringAt(value), self->lex->name);
            return AstNode_new(
                AstNodeType_LITERAL,
                value,
                self->loc,
                NULL
            );
        }       
        case LexemeType_CHARACTER:{
            Debug_suppose(String_length(self->lex->name) <= 1, "Char lexeme name length is != 1");
            DynamicType * value = DynamicType_new(Type_CHARACTER);
            DynamicType_setChar(value, String_at(self->lex->name, 0));
            return AstNode_new(
                AstNodeType_LITERAL,
                value,
                self->loc,
                NULL
            );
        }
        case LexemeType_FLOAT:{
            DynamicType * value = DynamicType_new(Type_FLOAT);
            DynamicType_setFloat(value, atof(String_readOnly(self->lex->name)));
            return AstNode_new(
                AstNodeType_LITERAL,
                value,
                self->loc,
                NULL
            );
        }
        default: return AstNode_new(AstNodeType_UNKNOWN, NULL, self->loc, NULL);
    }
}

static void Private_writeLineError(unsigned int lineStart, unsigned int x, Parser * parser){
    unsigned int i = lineStart;
    while (i < String_length(parser->code) && String_at(parser->code, i) != '\n'){
        String_appendChar(parser->error, String_at(parser->code, i));
        ++i;
    }
    String_appendChar(parser->error, '\n');
    i = 0;
    while (i < x - 1){
        String_appendChar(parser->error, ' ');
        ++i;
    }
    String_append(parser->error, "🠹\n");
}

static void Private_writeErrorRule(const char * expected, Parser * parser){
    Debug_exists(expected);
    //
    if (parser->error) return;
    parser->error = String_toNewString("Unresolved compilation problem ");
    if (!Iterator_inbound(parser->iter)){
        String_append(parser->error, "at the end of input\n");
    } else {
        Point2D loc = ((Token *)(Iterator_value(parser->iter)))->loc;
        String_appendFormat(parser->error, "(%i : %i)\n", loc.y, loc.x);
    }
    String_append(parser->error, expected);
    String_append(parser->error, " but experienced token ");
    if (!Iterator_inbound(parser->iter)){
        String_append(parser->error, "❲ NULL ❳");
    }
    else {
        Token_appendTo(parser->error, Iterator_value(parser->iter));
        String_appendChar(parser->error, '\n');
        Point2D loc = ((Token *)Iterator_value(parser->iter))->loc;
        signed long long lineStart = 0;
        if (loc.y > 1){
             lineStart = String_searchOccurence(parser->code, '\n', loc.y - 1) + 1;
        }
        Private_writeLineError(lineStart, loc.x, parser);
    }
}

static void Private_writeError(LexemeType expected, Parser * parser){
    parser->error = String_toNewString("Unresolved compilation problem ");
    if (!Iterator_inbound(parser->iter)){
        String_append(parser->error, "at the end of input\n");
    } else {
        Point2D loc = ((Token *)(Iterator_value(parser->iter)))->loc;
        String_appendFormat(parser->error, "(%i : %i)\n", loc.y, loc.x);
    }
    String_append(parser->error, "Expected token ❲ ");
    LexemeType_appendTo(parser->error, expected);
    String_append(parser->error, " ❳ but experienced ");
    if (!Iterator_inbound(parser->iter)){
        String_append(parser->error, "❲ NULL ❳");
    }
    else {
        Token_appendTo(parser->error, Iterator_value(parser->iter));
        String_appendChar(parser->error, '\n');
        Point2D loc = ((Token *)Iterator_value(parser->iter))->loc;
        signed long long lineStart = 0;
        if (loc.y > 1){
             lineStart = String_searchOccurence(parser->code, '\n', loc.y - 1) + 1;
        }
        Private_writeLineError(lineStart, loc.x, parser);
    }
}

static bool acceptDelimiter(Parser * parser, LexemeType tokenType){
    if (!Iterator_inbound(parser->iter) 
        || tokenType != Token_typeAt(Iterator_value(parser->iter))) return false;
    //
    Iterator_next(parser->iter);
    return true;
}

static bool expectDelimiter(Parser * parser, LexemeType tokenType){
    PARSER_EXPECTED_TRACE_CALL();
    return !acceptDelimiter(parser, tokenType) 
        ? (Private_writeError(tokenType, parser), false)
        : true;
}

// CHECK current token type
static Tree * accept(Parser * parser, LexemeType tokenType){
    CHECK(Iterator_inbound(parser->iter));
    Token * token = Iterator_value(parser->iter);
    CHECK(tokenType == Token_typeAt(token));
    Iterator_next(parser->iter);
    return Tree_new(Token_toNewAstNode(token));
}

// expect exactly that token type or fail
static Tree * expect(Parser * parser, LexemeType tokenType){
    PARSER_EXPECTED_TRACE_CALL();
    Tree * accepted = accept(parser, tokenType);
    return accepted
        ? accepted
        : (Private_writeError(tokenType, parser), NULL);
}


static bool __Ebnf_multiple (Parser * parser, Tree * root, GrammarRule rule){
    Debug_exists(root);
    Tree * child = NULL;
    while ((child = rule(parser))){
        Tree_addLastChild(root, child);
    }
    if (parser->error){
        Tree_freeWhole(root, (Destructor)AstNode_freeWhole);
        return false;
    }
    else return true;
}

static Tree *  __Ebnf_select(Parser * parser, GrammarRule rules[], unsigned char size){
    for (unsigned char i = 0; i < size; ++i){
        Tree * tree = NULL;
        if((tree = rules[i](parser))) 
            return tree;
        else 
            CHECK(!parser->error, tree);
    }
    return NULL;
}

static Tree *  __Ebnf_selectLexeme(Parser * parser, LexemeType types[], unsigned char size){
    for (unsigned char i = 0; i < size; ++i){
        Tree * lexeme = NULL;
        if ((lexeme = accept(parser, types[i]))) return lexeme;
    }
    return NULL;
}

static Tree *  Ebnf_apMainRule(Parser * parser, GrammarRule higherRule, GrammarRule apRule){
    Debug_exists(higherRule); Debug_exists(apRule);
    //
    Tree * leftOperand = higherRule(parser);
    CHECK(leftOperand);
    Tree * top = apRule(parser);
    if (top){
        Tree * cur = top;
        while (Tree_childrenSize(cur) != 1){
            cur = Tree_childAt(cur, 0);
        }
        Tree_addFirstChild(cur, leftOperand);
        return top;
    } else {
        CHECK(!parser->error, leftOperand);
        return leftOperand;
    }
}

static Tree *  __Ebnf_apRule(Parser * parser, GrammarRule higherRule, 
                          LexemeType * lexems, unsigned char size){
    Debug_exists(higherRule); Debug_exists(lexems);
    //
    Tree * operator = __Ebnf_selectLexeme(parser, lexems, size);
    CHECK(operator);
    
    Tree * operand = higherRule(parser);  // skazal A, skazhi B
    EXPECT_RULE(operand, operator, 
            "Expected expression after binary operator ");
    Tree_addFirstChild(operator, operand);

    Tree * top = __Ebnf_apRule(parser, higherRule, lexems, size);
    if (top){
        Tree * cur = top;
        while (Tree_childrenSize(cur) == 2){
            cur = Tree_childAt(cur, 0);
        }
        Tree_addFirstChild(cur, operator); // a and b are operands of higher precendece
        return top;
    } else {                               // [ ] - brackets define current function scope
        // case ... + a [ +  b  EPSILON ] - recursion delimiter                     
        CHECK(!parser->error, operator);
        return operator;                                   
    }
}                                                       

static Tree * Calamity        (Parser * parser);
static Tree * funcdef         (Parser * parser);
static Tree * id_list         (Parser * parser);
static Tree * id_enum         (Parser * parser);
static Tree * statement       (Parser * parser);
static Tree * vardecl         (Parser * parser);
static Tree * initialization  (Parser * parser);
static Tree * alter_st        (Parser * parser);
static Tree * otherwise_st    (Parser * parser);
static Tree * cycle_st        (Parser * parser);
static Tree * block_st        (Parser * parser);
static Tree * blank_st        (Parser * parser);
static Tree * return_st       (Parser * parser);
static Tree * expression_st   (Parser * parser);
static Tree * expression      (Parser * parser);
static Tree * assign          (Parser * parser);
static Tree * assign_ap       (Parser * parser);
static Tree * concatenate     (Parser * parser);
static Tree * concatenate_ap  (Parser * parser);
static Tree * or              (Parser * parser);
static Tree * or_ap           (Parser * parser);
static Tree * and             (Parser * parser);
static Tree * and_ap          (Parser * parser);
static Tree * equaling        (Parser * parser);
static Tree * equaling_ap     (Parser * parser);
static Tree * comparison      (Parser * parser);
static Tree * comparison_ap   (Parser * parser);
static Tree * add             (Parser * parser);
static Tree * add_ap          (Parser * parser);
static Tree * mult            (Parser * parser);
static Tree * mult_ap         (Parser * parser);
static Tree * unary           (Parser * parser);
static Tree * power           (Parser * parser);
static Tree * power_ap        (Parser * parser);
static Tree * list_call       (Parser * parser);
static Tree * list_call_ap    (Parser * parser);
static Tree * primary         (Parser * parser);
static Tree * literal         (Parser * parser);
static Tree * ch_str_float    (Parser * parser);
static Tree * bool_or_undef   (Parser * parser);
static Tree * compound_literal(Parser * parser);
static bool   expr_list       (Parser * parser, Tree * root);
static Tree * expr_enum       (Parser * parser);
static Tree * parentheses     (Parser * parser);
static Tree * var_or_call     (Parser * parser);
static Tree * var_or_call_ap  (Parser * parser);
static Tree * list_operator   (Parser * parser);
static Tree * list_operator_ap(Parser * parser);
static Tree * function_call   (Parser * parser);
static Tree * argument_list   (Parser * parser);
//
static Tree * IDENTIFIER      (Parser * parser);



Tree * Parser_buildNewAstTree(const register List * tokens, const String * code){
    Debug_exists(tokens);
    //
    auto Parser parser = {
        .iter = List_getNewBeginIterator(tokens),
        .error = NULL,
        .code = code
    };
    register Parser * const speedUpPerformance = &parser;
    Tree * tree = Calamity(speedUpPerformance);
    if (parser.error){
        Cui_setFgColorOf(stderr, 500);
        String_writeTo(stderr, parser.error);
        String_free(parser.error);
        fputc('\n', stderr);
        Iterator_free(parser.iter);
        return NULL;
    }
    else if (Iterator_inbound(parser.iter)){
        Private_writeErrorRule("Didn't expect it,", &parser);
        //
        Debug_exists(parser.error);
        //
        Cui_setFgColorOf(stderr, 500);
        String_writeTo(stderr, parser.error);
        String_free(parser.error);
        fputc('\n', stderr);
        Iterator_free(parser.iter);
        if (tree) Tree_freeWhole(tree, (Destructor)AstNode_freeWhole);
        return NULL;
    }
    Iterator_free(parser.iter);
    return tree;
}





static Tree * Calamity       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * calamity = Tree_newAstConstruct(AstNodeType_CALAMITY);
    CHECK(Ebnf_multiple(parser, calamity, funcdef));
    size_t children;
    do {
        children = Tree_childrenSize(calamity);
        CHECK(Ebnf_multiple(parser, calamity, vardecl));
        CHECK(Ebnf_multiple(parser, calamity, statement));
    } while (Tree_childrenSize(calamity) != children);
    return calamity;
}
static Tree * funcdef         (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_DEFINE));
    Tree * definition = Tree_newAstConstruct(AstNodeType_DEFINE);
    Tree * funcId = expect(parser, LexemeType_IDENTIFIER);
    CHECK(funcId, definition);
    Tree_addLastChild(definition, funcId);
    CHECK(expectDelimiter(parser, LexemeType_LEFT_PAR), definition);
    Tree * arguments = id_list(parser);
    EXPECT_RULE(arguments, definition, 
        "Expected arguments id after '(' in function definition");
    Tree_addLastChild(funcId, arguments);
    CHECK(expectDelimiter(parser, LexemeType_RIGHT_PAR), definition);
    Tree * body = statement(parser);
    EXPECT_RULE(body, definition, "Expected function body after definition");
    Tree_addLastChild(definition, body);
    return definition;    
}
static Tree * id_list         (Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * idList = Tree_newAstConstruct(AstNodeType_ARGLIST);
    Tree * arg = IDENTIFIER(parser);
    if (!arg) return idList;
    Tree_addFirstChild(idList, arg);
    CHECK(Ebnf_multiple(parser, idList, id_enum));
    return idList;
}
static Tree * id_enum         (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_COMA));
    Tree * id = IDENTIFIER(parser);
    EXPECT_RULE(id, NULL, "Expected idetifier after ','");
    return id;
}
static Tree * statement      (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_select(parser, {
        vardecl,
        alter_st, 
        cycle_st, 
        block_st, 
        blank_st,
        expression_st,
        return_st
    });
}
static Tree * vardecl        (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_NEW));
    Tree * identifier = expect(parser, LexemeType_IDENTIFIER);
    CHECK(identifier, NULL);
    Tree * declaration = Tree_newAstConstruct(AstNodeType_DECLARATION);
    Tree_addFirstChild(declaration, identifier);
    //
    Tree * initializer = initialization(parser);
    OPTIONAL(initializer, declaration ,declaration, initializer);
    CHECK(expectDelimiter(parser, LexemeType_TERMINATING), declaration);
    return declaration;
}
static Tree * initialization (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_ASSIGN), NULL);
    Tree * expr = expression(parser);
    EXPECT_RULE(expr, NULL, "Expected expression near initialization");
    return expr;
}
static Tree * alter_st       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * _case = accept(parser, LexemeType_CASE);                      // case
    CHECK(_case && expectDelimiter(parser, LexemeType_LEFT_PAR), _case); // (
    //
    Tree * cond = expression(parser);       
    EXPECT_RULE(cond, _case, "Expected condition after 'case'");      // condition
    Tree_addFirstChild(_case, cond);
    //
    CHECK(expectDelimiter(parser, LexemeType_RIGHT_PAR), _case);       // )
    //
    Tree * caseTrue = statement(parser);                              // statement
    EXPECT_RULE(caseTrue, _case, "Expected statement after case ()");
    Tree_addLastChild(_case, caseTrue);
    Tree * otherwise = otherwise_st(parser);     
    OPTIONAL(otherwise, _case, _case, otherwise);                     // otherwise
    return _case;
}
static Tree * otherwise_st   (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_OTHERWISE), NULL);
    Tree * caseFalse = statement(parser);
    EXPECT_RULE (caseFalse, NULL, "Expected statement after 'otherwise'");
    return caseFalse;
}
static Tree * cycle_st       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * _while = accept(parser, LexemeType_WHILE);
    CHECK(_while && expectDelimiter(parser, LexemeType_LEFT_PAR), _while);

    Tree * expr = expression(parser);
    EXPECT_RULE(expr, _while, "Expected condition after 'while'");
    Tree_addLastChild(_while, expr);

    CHECK(expectDelimiter(parser, LexemeType_RIGHT_PAR), _while);

    Tree * st = statement(parser);
    EXPECT_RULE(st, _while, "Expected statement after while()");
    Tree_addLastChild(_while, st);
    return _while;
}
static Tree * block_st       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_BLOCK_START));
    Tree * block = Tree_newAstConstruct(AstNodeType_BLOCK);
    OPTIONAL(Ebnf_multiple(parser, block, statement), NULL, NULL, NULL);
    CHECK(expectDelimiter(parser, LexemeType_BLOCK_FINISH), block);
    return block;
}
static Tree * blank_st       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_TERMINATING));
    return Tree_newAstConstruct(AstNodeType_BLANK_STATEMENT);
}
static Tree * return_st       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_RETURN));
    Tree * ret = Tree_newAstConstruct(AstNodeType_RETURN);
    Tree * expr = expression(parser);
    EXPECT_RULE(expr, ret, "Expected expression after return");
    Tree_addLastChild(ret, expr);
    CHECK(expectDelimiter(parser, LexemeType_TERMINATING), ret);
    return ret;
}
static Tree * expression_st  (Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * expr = expression(parser);
    CHECK(expr && expectDelimiter(parser, LexemeType_TERMINATING), expr);
    return expr;
}
static Tree * expression     (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return assign(parser);
}
static Tree * assign         (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return  Ebnf_apMainRule(parser, concatenate, assign_ap);
}
static Tree * assign_ap      (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, concatenate, {
        LexemeType_ASSIGN,
        LexemeType_APPEND
    });
}
static Tree * concatenate    (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, or, concatenate_ap);
}
static Tree * concatenate_ap (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, or, {
        LexemeType_CONCATENATE
    });
}
static Tree * or             (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, and, or_ap);
}
static Tree * or_ap          (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, and, {
        LexemeType_OR
    });
}
static Tree * and            (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, equaling, and_ap);
}
static Tree * and_ap         (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, equaling, {
        LexemeType_AND
    });
}
static Tree * equaling       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, comparison, equaling_ap);
}
static Tree * equaling_ap    (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, comparison, {
        LexemeType_EQ,
        LexemeType_NEQ
    });
}
static Tree * comparison     (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, add, comparison_ap);
}
static Tree * comparison_ap  (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, add, {
        LexemeType_GREATER,
        LexemeType_LESS,
        LexemeType_GEQ,
        LexemeType_LEQ
    });
}
static Tree * add            (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, mult, add_ap);
}
static Tree * add_ap         (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, mult, {
        LexemeType_PLUS,
        LexemeType_MINUS
    });
}
static Tree * mult           (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, unary, mult_ap);
}
static Tree * mult_ap        (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, unary, {
        LexemeType_MULT,
        LexemeType_DIV,
        LexemeType_REMAINDER
    });
}
static Tree * unary          (Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * operator = Ebnf_selectLexeme(parser, {
        LexemeType_PLUS,
        LexemeType_MINUS,
        LexemeType_NOT
    });
    Tree * operand = power(parser);
    if (operator == NULL) {
        return operand;
    } else {
        EXPECT_RULE(operand, operator, "Expected operand for unary operator ");
        Tree_addLastChild(operator, operand);
        return operator;
    }
}
static Tree * power          (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apMainRule(parser, list_call, power_ap);
}
static Tree * power_ap       (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_apRule(parser, list_call, {
        LexemeType_POW
    });
}
static Tree * list_call(Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * prim = primary(parser);
    CHECK(prim);
    Tree * top = list_call_ap(parser);
    if (top){
        Tree * cur = top;
        while (Tree_childrenSize(cur) >= 2 && AstNode_typeAt(Tree_childAt(cur, 0)->val) == AstNodeType_PRIM_ARGLIST){
            cur = Tree_childAt(cur, 0);
        }
        Tree_addFirstChild(cur, prim);
        return top;
    } else {
        CHECK(!parser->error, prim);
        return prim;
    }
}
static Tree * list_call_ap(Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * operator = list_operator(parser);
    CHECK(operator);
    Tree * top = list_call_ap(parser);
    if (top){
        Tree * cur = top;
        while (Tree_childrenSize(cur) >= 2 && AstNode_typeAt(Tree_childAt(cur, 0)->val) == AstNodeType_PRIM_ARGLIST){
            cur = Tree_childAt(cur, 0);
        }
        Tree_addFirstChild(cur, operator);
        return top;
    } else {
        CHECK(!parser->error, operator);
        return operator;
    }
}
static Tree * primary        (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_select(parser, {
        literal,
        parentheses,
        var_or_call
    });
}
static Tree * literal        (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_select(parser, {
        bool_or_undef,
        ch_str_float,
        compound_literal
    });
}

static Tree * ch_str_float  (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_selectLexeme(parser, {
        LexemeType_CHARACTER,
        LexemeType_STRING,
        LexemeType_FLOAT,
        LexemeType_ENDL
    });
}
static Tree * bool_or_undef  (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return Ebnf_selectLexeme(parser, {
        LexemeType_UNDEFINED,
        LexemeType_TRUE,
        LexemeType_FALSE
    });
}
static Tree * compound_literal(Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_LEFT_BRACKET));
    Tree * compound = Tree_newAstConstruct(AstNodeType_COMPOUND_LITERAL);
    OPTIONAL(expr_list(parser, compound), NULL, NULL, NULL);
    CHECK(expectDelimiter(parser, LexemeType_RIGHT_BRACKET), compound);
    return compound;
}
static bool expr_list      (Parser * parser, Tree * root){
    PARSER_TRACE_CALL();
    //
    Tree * expr = expression(parser);
    CHECK(expr);
    Tree_addLastChild(root, expr);
    CHECK(Ebnf_multiple(parser, root, expr_enum));
    return true;
}
static Tree * expr_enum(Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_COMA));
    Tree * expr = expression(parser);
    EXPECT_RULE(expr, NULL, "Expected expression after ','");
    return expr;
}
static Tree * parentheses    (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_LEFT_PAR));
    Tree * expr = expression(parser);
    EXPECT_RULE(expr, NULL, "Expected expression after opened parentheses '('");
    CHECK(expectDelimiter(parser, LexemeType_RIGHT_PAR), expr);
    return expr;    
}
static Tree * var_or_call(Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * identifier = IDENTIFIER(parser);
    CHECK(identifier);
    Tree * call = function_call(parser);
    OPTIONAL(call, identifier, identifier, call);
    return identifier;
}
static Tree * list_operator  (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_LEFT_BRACKET));
    Tree * operand1 = expression(parser);
    EXPECT_RULE(operand1, NULL, "Expected argument after '[' near identifier");

    Tree * argList = Tree_newAstConstruct(AstNodeType_PRIM_ARGLIST);
    Tree_addLastChild(argList, operand1);

    Tree * operand2 = list_operator_ap(parser);
    OPTIONAL(operand2, argList, argList, operand2);

    return argList;
}
static Tree * list_operator_ap(Parser * parser){
    PARSER_TRACE_CALL();
    //
    // case enclosed getter bracket a[i]
    if (acceptDelimiter(parser, LexemeType_RIGHT_BRACKET)) return NULL;  
    //
    // case ranger operator a[i ... j]
    CHECK(expectDelimiter(parser, LexemeType_RANGE)); 
    Tree * operand2 = expression(parser);
    EXPECT_RULE(operand2, NULL, "Expected expression after '...'");
    CHECK(expectDelimiter(parser, LexemeType_RIGHT_BRACKET), operand2);

    return operand2;
}
static Tree * function_call  (Parser * parser){
    PARSER_TRACE_CALL();
    //
    CHECK(acceptDelimiter(parser, LexemeType_LEFT_PAR));
    Tree * argList = argument_list(parser);
    EXPECT_RULE(argList, argList, "Expected argument list for function identifier");
    CHECK(expectDelimiter(parser, LexemeType_RIGHT_PAR), argList);
    return argList;
}
static Tree * argument_list  (Parser * parser){
    PARSER_TRACE_CALL();
    //
    Tree * argList = Tree_newAstConstruct(AstNodeType_ARGLIST);
    OPTIONAL(expr_list(parser, argList), argList, NULL, NULL);
    return argList;
}
static Tree * IDENTIFIER     (Parser * parser){
    PARSER_TRACE_CALL();
    //
    return accept(parser, LexemeType_IDENTIFIER);
}
