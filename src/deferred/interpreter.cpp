#include <math.h>

#include "interpreter.h"
#include "dict.h"
#include "ast.h"
#include "error.h"
#include "std_function.h"
#include "interpreter_def.h"
#include "var.h"
#include "lexer.h"
#include "main_parser.h"
#include "cui.h"
#include "mem.h"


enum {
    Interpreter_SUCCESS,
    Interpreter_TERMINATE
};


typedef struct __UserDef{
    List * argv;
    Tree * body;
} UserDef;

static UserDef * UserDef_new(List * argv, Tree * body){
    UserDef * newbie = Mem_malloc(sizeof(UserDef));
    newbie->argv = argv;
    newbie->body = body;
    return newbie;
}

static void UserDef_freeWhole(UserDef * self){
    Debug_exists(self); 
    //
    List_freeWhole(self->argv, (Destructor)String_free);
    free(self);
}

static void freeLocals(Calamity * prog){
    for (size_t i = 0; i < List_size(prog->locals); ++i){
        Var_free(Dict_remove(prog->vars, String_readOnly(List_at(prog->locals, i))));
    }
}

Var * evaluate_new(Calamity * prog, Tree * ast);

#define CHECK(...)                   __CHECK(Interpreter_TERMINATE, __VA_ARGS__)
#define EXPECT(expr, errorMsg, ...) __EXPECT(Interpreter_TERMINATE, expr, errorMsg, __VA_ARGS__)
static int execute(Calamity * prog, const Tree * ast){
    switch (AstNode_typeAt(ast->val)){
        case AstNodeType_DEFINE:{
            const char * name = String_readOnly(AstNode_nameAt(Tree_at(Tree_childAt(ast, 0))));
            EXPECT(!Dict_contains(prog->funcs, name), 
                "declaration is incompatible as redefining standart function id", 
                Tree_childAt(ast, 0)
            );
            EXPECT(!Dict_contains(prog->vars, name), 
                "declaration is incompatible as redefining variable id", 
                Tree_childAt(ast, 0)
            );
            EXPECT(!Dict_contains(prog->defs, name),
                "declaration is incompatible as redefining existing function id",
                Tree_childAt(ast, 0)
            );
            UserDef * def = UserDef_new(List_new(), Tree_childAt(ast, 1));
            Tree * argv = Tree_childAt(Tree_childAt(ast, 0), 0);
            Dict * checkDuplicated = Dict_new();
            for (size_t i = 0; i < Tree_childrenSize(argv); ++i){
                const String * name = AstNode_nameAt(Tree_childAt(argv, i)->val);
                if (Dict_contains(checkDuplicated, String_readOnly(name))){
                    Calamity_setError("redecalration of argument at function definition", Tree_childAt(argv, i)->val, prog);
                    Dict_free(checkDuplicated);
                    UserDef_freeWhole(def);
                    return Interpreter_TERMINATE;
                }
                List_addLast(def->argv, String_newCopyOf(name));
                Dict_set(checkDuplicated, String_readOnly(name), NULL);
            }
            Dict_free(checkDuplicated);
            Dict_set(prog->defs, String_readOnly(AstNode_nameAt(Tree_childAt(ast, 0)->val)), def);
            return Interpreter_SUCCESS;
        }
        case AstNodeType_RETURN:{
            Var * retVal = evaluate_new(prog, Tree_childAt(ast, 0));
            CHECK(retVal);
            prog->status = retVal;
            prog->state = ProgState_RETURN;
            return Interpreter_TERMINATE;            
        }
        case AstNodeType_DECLARATION:{
            Debug_maxbound(Tree_childrenSize(ast), 2);
            //
            String * name = AstNode_nameAt(Tree_at(Tree_childAt(ast, 0)));
            EXPECT(!Dict_contains(prog->funcs, String_readOnly(name)), 
                "declaration is incompatible as redefining standart function id", 
                Tree_childAt(ast, 0)
            );
            EXPECT(!Dict_contains(prog->vars, String_readOnly(name)), 
                "redefiniton of variable", 
                Tree_childAt(ast, 0)
            );
            List_addLast(prog->locals, name);
            if (Tree_childrenSize(ast) == 1){

                Dict_set(prog->vars, String_readOnly(name), Var_new(Type_UNDEFINED));

            } else {
                Var * initializer = evaluate_new(prog, Tree_childAt(ast, 1));
                CHECK(initializer);
                Dict_set(prog->vars, String_readOnly(name),initializer);

            }
            return Interpreter_SUCCESS;
        }

        case AstNodeType_BLANK_STATEMENT: return Interpreter_SUCCESS;
        case AstNodeType_BLOCK: {
            List * externLocals = prog->locals;
            prog->locals = List_new();
            for (size_t i = 0; i < Tree_childrenSize(ast); ++i){
                if (execute(prog, Tree_childAt(ast, i)) == Interpreter_TERMINATE){
                    freeLocals(prog);
                    List_free(prog->locals);
                    prog->locals = externLocals;
                    return Interpreter_TERMINATE;
                }
            }            
            freeLocals(prog);
            List_free(prog->locals);
            prog->locals = externLocals;
            return Interpreter_SUCCESS;
        }
        case AstNodeType_CASE:{
            Debug_maxbound(Tree_childrenSize(ast), 3);
            //
            Var * condition = evaluate_new(prog, Tree_childAt(ast, 0));
            CHECK(condition);
            EXPECT(Var_typeAt(condition) != Type_UNDEFINED,
                     "if condition results in undefined value", 
                        ast, 
                            condition);
            //
            // TODO: TUTA PAHNE KOSTYLEM
            if (Var_convertToBool(condition)){
                Var_free(condition);
                if (AstNode_typeAt(Tree_childAt(ast, 1)->val) != AstNodeType_DECLARATION){
                    return execute(prog, Tree_childAt(ast, 1));
                } else { // case single vardecl statement remove local variable after execution
                    int res = execute(prog, Tree_childAt(ast, 1));
                    Var_free(Dict_remove(prog->vars, String_readOnly(List_removeLast(prog->locals))));
                    return res;
                }
                
            } else if (Tree_childrenSize(ast) == 3){
                Var_free(condition);
                if (AstNode_typeAt(Tree_childAt(ast, 2)->val) != AstNodeType_DECLARATION){
                    return execute(prog, Tree_childAt(ast, 2));
                } else { // case single vardecl statement
                    int res = execute(prog, Tree_childAt(ast, 2));
                    Var_free(Dict_remove(prog->vars, String_readOnly(List_removeLast(prog->locals))));
                    return res;
                }
            } else {
                Var_free(condition);
                return Interpreter_SUCCESS;
            }
        }
        case AstNodeType_WHILE:{
            Debug_suppose(Tree_childrenSize(ast) == 2, "while without condtion or statement");
            //
            do {

                Var * condition = evaluate_new(prog, Tree_childAt(ast, 0));
                CHECK(condition);
                EXPECT(Var_typeAt(condition) != Type_UNDEFINED,
                        "while condition results in undefined value", 
                            ast, 
                                condition
                );

                if (Var_convertToBool(condition)){
                    Var_free(condition);
                    if (AstNode_typeAt(Tree_childAt(ast, 1)->val) != AstNodeType_DECLARATION){
                        CHECK(execute(prog, Tree_childAt(ast, 1)) == Interpreter_SUCCESS);
                    } else {  //single vardecl statement remove local variable after execution
                        CHECK(execute(prog, Tree_childAt(ast, 1)) == Interpreter_SUCCESS);
                        Var_free(Dict_remove(prog->vars, String_readOnly(List_removeLast(prog->locals))));
                    }
                }
                else {
                    Var_free(condition);
                    return Interpreter_SUCCESS;
                }


            } while (true);
        }
        case AstNodeType_UNKNOWN: EXPECT(false, "unknown syntax", ast);
        default: {
            Var * res = evaluate_new(prog, (Tree *)ast);
            if (res){
                Var_free(res);
                return Interpreter_SUCCESS;
            } else {
                return Interpreter_TERMINATE;
            }
        }
    }
    // VVV this must never happen (workflow terminates at default case in switch)
    return Interpreter_TERMINATE;
}


Var * Interpreter_execute(const Tree * astTree, const String * code){
    Debug_exists(astTree);
    Debug_exists(code);
    Calamity prog = {
        .vars   = Dict_new(),
        .funcs  = Dict_new(),
        .defs   = Dict_new(),
        .error  = NULL,
        .code   = code,
        .locals = List_new(),
        .status = NULL,
        .state  = ProgState_IDLE
    }; 
    // setting standart functions identifiers
    Dict_set(prog.funcs, "digify",    Function_new(std_digify));
    Dict_set(prog.funcs, "insert",    Function_new(std_insert));
    Dict_set(prog.funcs, "print",     Function_new(std_print));
    Dict_set(prog.funcs, "write",     Function_new(std_write));
    Dict_set(prog.funcs, "read",      Function_new(std_read));
    Dict_set(prog.funcs, "remove",    Function_new(std_remove));
    Dict_set(prog.funcs, "sizeof",    Function_new(std_sizeof));
    Dict_set(prog.funcs, "sqrt",      Function_new(std_sqrt));
    Dict_set(prog.funcs, "stringify", Function_new(std_stringify));
    Dict_set(prog.funcs, "typeof",    Function_new(std_typeof));
    Dict_set(prog.funcs, "escape",    Function_new(std_escape));
    Dict_set(prog.funcs, "array",     Function_new(std_array));
    for (size_t i = 0; i < Tree_childrenSize(astTree); ++i){
        if (execute(&prog, Tree_childAt(astTree, i)) == Interpreter_TERMINATE) {
            if (prog.state == ProgState_ERROR){
                //
                Debug_exists(prog.error);
                Debug_suppose(!prog.status, "status returned non-null value at error report");
                //
                prog.status = Var_new(Type_STRING);
                Var_setString(prog.status, prog.error);
                Dict_freeWhole(prog.vars,  (Destructor)Var_free);
                Dict_freeWhole(prog.funcs, (Destructor)Function_free);
                Dict_freeWhole(prog.defs,  (Destructor)UserDef_freeWhole);
                List_free(prog.locals);
                return prog.status;
            }
            Debug_exists(prog.status);
            //
            Dict_freeWhole(prog.vars,  (Destructor)Var_free);
            Dict_freeWhole(prog.funcs, (Destructor)Function_free);
            Dict_freeWhole(prog.defs,  (Destructor)UserDef_freeWhole);
            List_free(prog.locals);
            return prog.status;
        }
    }
    Debug_suppose(prog.error == NULL, "An attempt to return success status with error string was made");
    //
    Dict_freeWhole(prog.vars,  (Destructor)Var_free);
    Dict_freeWhole(prog.funcs, (Destructor)Function_free);
    Dict_freeWhole(prog.defs,  (Destructor)UserDef_freeWhole);
    List_free(prog.locals);
    return NULL;
}


#define CHECK_UNDEF_EXPANSION(value, ...)                     \
    if (Var_typeAt(value) == Type_UNDEFINED) {        \
        Var * ___freeArr[] = {__VA_ARGS__};           \
        MACRO_FREE_DYNTYPE_ARR(___freeArr,                    \
                            sizeof(___freeArr)                \
                            / sizeof(Var *));         \
        return value;                                         \
    }                                                         \

// return undefined value if value as its first argument is undefined,
// but before free all the values after it
#define CHECK_UNDEF(...) CHECK_UNDEF_EXPANSION(__VA_ARGS__, NULL)

#undef CHECK
#undef EXPECT
#define CHECK(...)                   __CHECK(NULL, __VA_ARGS__)
#define EXPECT(expr, errorMsg, ...) __EXPECT(NULL, expr, errorMsg, __VA_ARGS__)

 
bool greater(Var * l, Var * r){ return Var_compare(l, r) == -1; }
bool geq    (Var * l, Var * r){ return Var_compare(l, r) !=  1; }

bool eq     (Var * l, Var * r){
    return Var_typeAt(l) == Var_typeAt(r)
            ? Var_compare(l, r) ==  0 : false;
}

bool neq    (Var * l, Var * r){
    return Var_typeAt(l) == Var_typeAt(r)
            ? Var_compare(l, r) !=  0 : true;
}

bool less   (Var * l, Var * r){ return Var_compare(l, r) ==  1; }
bool leq    (Var * l, Var * r){ return Var_compare(l, r) != -1; }
bool and    (Var * l, Var * r){
    return Var_convertToBool(l) && Var_convertToBool(r);
}
bool or(Var * l, Var * r){
    return Var_convertToBool(l) || Var_convertToBool(r);
}

typedef bool (*BinaryLogic)(Var * l, Var * r);

static Var * getNewBinaryComparisonResult(Calamity * prog, Tree * ast, BinaryLogic compare){
    Var * opeL = evaluate_new(prog, Tree_childAt(ast, 0));
    CHECK(opeL);
    Var * opeR = evaluate_new(prog, Tree_childAt(ast, 1));
    CHECK(opeR, opeL);

    CHECK_UNDEF(opeL, opeR);
    CHECK_UNDEF(opeR, opeL);

    EXPECT(Var_typeAt(opeL) == Var_typeAt(opeR),
        "different type operands for binary comparison operator", ast, opeR, opeL);

    Var_setBool(opeL, compare(opeL, opeR));
    Var_free(opeR);
    return opeL;
}

static Var * getNewBinaryEqualingResult(Calamity * prog, Tree *ast, BinaryLogic compare){
    Var * opeL = evaluate_new(prog, Tree_childAt(ast, 0));
    CHECK(opeL);
    Var * opeR = evaluate_new(prog, Tree_childAt(ast, 1));
    CHECK(opeR, opeL);

    Var_setBool(opeL, compare(opeL, opeR));
    Var_free(opeR);
    return opeL;
}

static Var * getNewLogBinaryOperatorResult(Calamity * prog, Tree * ast, BinaryLogic compare){
    Var * opeL = evaluate_new(prog, Tree_childAt(ast, 0));
    CHECK(opeL);
    Var * opeR = evaluate_new(prog, Tree_childAt(ast, 1));
    CHECK(opeR, opeL);
    
    CHECK_UNDEF(opeL, opeR);
    CHECK_UNDEF(opeR, opeL);

    Var_setBool(opeL, compare(opeL, opeR));
    Var_free(opeR);
    return opeL;
}

typedef char (*CharOperator)(char l, char r);
typedef float (*FloatOperator)(float l, float r);


static char subtractChars (char l, char r){ return l - r; }
static char addChars      (char l, char r){ return l + r; }
static char divideChars   (char l, char r){ return l / r; }
static char multiplyChars (char l, char r){ return l * r; }
static char remainderChars(char l, char r){ return l % r; }
static char powerChars    (char l, char r){
    if (r == 0) return (char)1;
    char powr = l;
    while (r > 1){
        powr *= l;
        --r;
    }
    return powr;
}
static float subtractFloats (float l, float r){ return l - r; }
static float addFloats      (float l, float r){ return l + r; }
static float divideFloats   (float l, float r){ return l / r; }
static float multiplyFloats (float l, float r){ return l * r; }
static float remainderFloats(float l, float r){ // this function was developed by Anatoliy Sanzharovskiy, Telegram: @usuallyNotATeamPlayer
    l /= r;
    l -= (int)l; 
    return (l + (l < 0)) * r;
}
static float powerFloats    (float l, float r){
    return (float)pow(l, r);
}

static Var * getNewNumBinaryDivOpertorResult(Calamity * prog, Tree * ast, CharOperator charOper, FloatOperator floatOper){
    Var * opeL = evaluate_new(prog, Tree_childAt(ast, 0));
    CHECK(opeL);                                                                                           
                                                                                                                
    Var * opeR = evaluate_new(prog, Tree_childAt(ast ,1));
    CHECK(opeR, opeL);

    CHECK_UNDEF(opeL, opeR);                                                                                    
    CHECK_UNDEF(opeR, opeL);                                                                                    
    EXPECT(Var_isNumeric(opeL), "invalid left operand type for binary operator", ast, opeL, opeR);
    EXPECT(Var_isNumeric(opeR), "invalid right operand type for binary operator", ast, opeL, opeR);
    Var_castBinaryNumOperands(opeL, opeR);
    switch (Var_typeAt(opeL)){
        case Type_CHARACTER:{                                                                                  
            EXPECT(Var_charAt(opeR) != 0, "division by zero exception", ast, opeL, opeR);
            Var_setChar(opeL, charOper(Var_charAt(opeL), Var_charAt(opeR)));
            break;
        }                                                                                                       
        default :{                                                                               
            EXPECT(Var_floatAt(opeR) != 0, "division by zero exception", ast, opeL, opeR);
            Var_setFloat(opeL, floatOper(Var_floatAt(opeL), Var_floatAt(opeR)));
            break;                                                                                      
        }                                                                                                       
    }  
    Var_free(opeR);
    return opeL;                                                                                               
}

static Var * getNewNumBinaryOpertorResult(Calamity * prog, Tree * ast, CharOperator charOper, FloatOperator floatOper){
    Var * opeL = evaluate_new(prog, Tree_childAt(ast, 0));
    CHECK(opeL);                                                                                           
                                                                                                                
    Var * opeR = evaluate_new(prog, Tree_childAt(ast ,1));
    CHECK(opeR, opeL);

    CHECK_UNDEF(opeL, opeR);                                                                                    
    CHECK_UNDEF(opeR, opeL);                                                                                    
    EXPECT(Var_isNumeric(opeL), "invalid left operand type for binary operator", ast, opeL, opeR);
    EXPECT(Var_isNumeric(opeR), "invalid right operand type for binary operator", ast, opeL, opeR);
    Var_castBinaryNumOperands(opeL, opeR);
    switch (Var_typeAt(opeL)){
        case Type_CHARACTER:{                                                                                   
            Var_setChar(opeL, charOper(Var_charAt(opeL), Var_charAt(opeR)));
            break;
        }                                                                                                       
        default :{                                                                                              
            Var_setFloat(opeL, floatOper(Var_floatAt(opeL), Var_floatAt(opeR)));
            break;                                                                                      
        }                                                                                                       
    }  
    Var_free(opeR);
    return opeL;                                                                                               
}                       

static char invertChar(char ch)  {return (char)-ch;}
static float invertFloat(float f)  {return -f;}

typedef char (*CharUnary)(char ch);
typedef float(*FloatUnary)(float f);

static Var * getNewNumUnaryOperatorResult(Calamity * prog, Tree * ast, CharUnary charOper, FloatUnary floatOper){
    Var * operand = evaluate_new(prog, Tree_childAt(ast, 0));
    if (!operand) return NULL;
    CHECK_UNDEF(operand);
    if (!Var_isNumeric(operand)){
        Calamity_setError("invalid operand type for unary operator", ast->val, prog); 
        Var_free(operand);
        return NULL;
    }
    switch (Var_typeAt(operand)){
        case Type_CHARACTER:{
            Var_setChar(operand, charOper(Var_charAt(operand)));
            return operand;
        }
        default:{
            Var_setFloat(operand, floatOper(Var_floatAt(operand)));
            return operand;
        }
    }
}

static Var * UserDef_exec(Calamity * prog, Tree * ast);

Var * evaluate_new(Calamity * prog, Tree * ast){
    switch (AstNode_typeAt(ast->val)){
        case AstNodeType_LITERAL:{
            return Var_newCopyOf(AstNode_valAt(ast->val));
        }
        case AstNodeType_AND:{
            return getNewLogBinaryOperatorResult(prog, ast, and);
        }
        case AstNodeType_ASSIGN:{
            Var * assigner = evaluate_new(prog, Tree_childAt(ast, 1));
            CHECK(assigner);
            return Calamity_setVarStorage(prog, Tree_childAt(ast, 0), assigner);
        }
        case AstNodeType_APPEND:{
            Var * appending = evaluate_new(prog, Tree_childAt(ast, 1));
            CHECK(appending);
            return Calamity_appendVarStorage(prog, Tree_childAt(ast, 0), appending);
        }
        case AstNodeType_COMPOUND_LITERAL:{
            Var * list = Var_new(Type_VECTOR);

            for (size_t i = 0; i < Tree_childrenSize(ast); ++i){
                Var * entity = evaluate_new(prog, Tree_childAt(ast, i));
                CHECK(entity, list);
                List_addLast(Var_listAt(list), entity);
            }
            return list;
        }
        case AstNodeType_CONCATENATE:{
            Var * opeL = evaluate_new(prog, Tree_childAt(ast, 0));
            CHECK(opeL);
            Var * opeR = evaluate_new(prog, Tree_childAt(ast, 1));
            CHECK(opeR, opeL);
            
            return Var_memUniteConcatenate(opeL, opeR);
        }
        case AstNodeType_DIV: {
            return getNewNumBinaryDivOpertorResult(prog, ast, divideChars, divideFloats);
        }
        case AstNodeType_EQ:{
            return getNewBinaryEqualingResult(prog, ast, eq);
        }
        case AstNodeType_GEQ:{
            return getNewBinaryComparisonResult(prog, ast, geq);
        }
        case AstNodeType_GREATER:{
            return getNewBinaryComparisonResult(prog, ast, greater);
        }
        case AstNodeType_IDENTIFIER:{

            const char * name = String_readOnly(AstNode_nameAt(Tree_at(ast)));

            if (Dict_contains(prog->vars, name)){
                EXPECT(Tree_childrenSize(ast) == 0, "variable identifier used as function", ast);
                return Var_newCopyOf(Dict_get(prog->vars, name));
            } else if (Dict_contains(prog->funcs, name)){
                EXPECT(Tree_childrenSize(ast) == 1, "parentheses after function identifier are not provided", ast)
                Tree * argList = Tree_childAt(ast, 0);
                EXPECT(AstNode_typeAt(argList->val) == AstNodeType_ARGLIST,
                        "function identifier used with list operator", ast);
                return Function_at(Dict_get(prog->funcs, name))(prog, ast);
            } else {
                EXPECT(Dict_contains(prog->defs, name), "use of undeclared identifier", ast);
                return UserDef_exec(prog, ast);
            }
        }
        case AstNodeType_PRIM_ARGLIST:{
            switch (Tree_childrenSize(ast)){
                case 2:{
                    return std_getter(prog, ast);
                }
                case 3:{
                    return std_ranger(prog, ast);
                }
                default:{
                    Debug_shutdown("Prim arglist has neither 2 nor 3 arguments");
                }
            }
        }
        case AstNodeType_LEQ:{
            return getNewBinaryComparisonResult(prog, ast, leq);
        }
        case AstNodeType_LESS:{
            return getNewBinaryComparisonResult(prog, ast, less);
        }
        case AstNodeType_MINUS:{
            switch (Tree_childrenSize(ast)){
                case 1:{
                    return getNewNumUnaryOperatorResult(prog, ast, invertChar, invertFloat);
                }
                case 2:{
                    return getNewNumBinaryOpertorResult(prog, ast, subtractChars, subtractFloats);
                }
            }
        }
        case AstNodeType_MULT:{
            return getNewNumBinaryOpertorResult(prog, ast, multiplyChars, multiplyFloats);
        }
        case AstNodeType_NEQ:{
            return getNewBinaryEqualingResult(prog, ast, neq);
        }
        case AstNodeType_NOT:{
            Var * operand = evaluate_new(prog, Tree_childAt(ast, 0));
            CHECK(operand);
            CHECK_UNDEF(operand);
            Var_setBool(operand, !Var_convertToBool(operand));
            return operand;
        }
        case AstNodeType_OR:{
            return getNewLogBinaryOperatorResult(prog, ast, or);
        }
        case AstNodeType_PLUS:{
            if (Tree_childrenSize(ast) == 1) return evaluate_new(prog, Tree_childAt(ast, 1));
            return getNewNumBinaryOpertorResult(prog, ast, addChars, addFloats);
        }
        case AstNodeType_POW:{
            return getNewNumBinaryOpertorResult(prog, ast, powerChars, powerFloats);
        }
        case AstNodeType_REMAINDER:{
            return getNewNumBinaryDivOpertorResult(prog, ast, remainderChars, remainderFloats);
        }
        default:{
            Debug_shutdown("Not implemented");
            return NULL;
        }
    }
    return NULL;
}


static Var * UserDef_exec(Calamity * prog, Tree * ast){
    EXPECT(Tree_childrenSize(ast) == 1, "parentheses after user function identifier are not provided", ast);
    Tree * arglist = Tree_childAt(ast, 0);

    UserDef * def = Dict_get(prog->defs, String_readOnly(AstNode_nameAt(ast->val)));
    EXPECT(AstNode_typeAt(arglist->val) == AstNodeType_ARGLIST,
        "user function identifier used with list operator", ast);
    EXPECT(Tree_childrenSize(arglist) == List_size(def->argv),
        "incompatible arguments count for user defined function", ast);

    Calamity routine = {
        .vars = Dict_new(),
        .funcs = prog->funcs,
        .defs = prog->defs,
        .error = NULL,
        .code = prog->code,
        .locals = List_new(),
        .status = NULL,
        .state = ProgState_IDLE
    };

    for (size_t i = 0; i < Tree_childrenSize(arglist); ++i){
        Var * val = evaluate_new(prog, Tree_childAt(arglist, i));
        if(!val){
            List_free(routine.locals);
            Dict_freeWhole(routine.vars, (Destructor)Var_free);
            return NULL;        
        }
        const char * name = String_readOnly(List_at(def->argv, i));
        Dict_set(routine.vars, name, val);
    }
    int res = execute(&routine, def->body);
    Dict_freeWhole(routine.vars, (Destructor)Var_free);
    List_free(routine.locals);
    switch (routine.state){
        case ProgState_ERROR:{       
            Debug_exists(routine.error);
            prog->error = routine.error;
            prog->state = ProgState_ERROR;
            Debug_suppose(routine.status == NULL, "returned non-NULL status while raising runtime exception");
            return NULL;
        }
        case ProgState_IDLE:{
            Debug_suppose(res == Interpreter_SUCCESS, "Non-success result for NULL status");
            //
            return Var_new(Type_UNDEFINED);
        } 
        case ProgState_ESCAPE:{
            prog->status = routine.status;
            prog->state = ProgState_ESCAPE;
            return NULL;
        }
        case ProgState_RETURN:{
            Debug_suppose(res == Interpreter_TERMINATE, "Non-terminate reslut for non-NULL status");
            //
            return routine.status;
        }
        default:{
            Debug_shutdown("Unknown prog state signal received");
        }
    }
}












Var * Calamity_execute(const String * code){
    setlocale(LC_CTYPE, "");
    List * tokens = List_new();
    if (Lexer_splitTokens(String_readOnly(code), tokens) != LEXER_SUCCESS){
        List_free(tokens);
        Var * err = Var_new(Type_STRING);
        String_append(Var_stringAt(err), "LexicalInfo error");
        return err;
    }
    Tree * ast = Parser_buildNewAstTree(tokens, code);
    if (ast) {
        Var * status = Interpreter_execute(ast, code);
        //
        // releasing memory
        Tree_freeWhole(ast, (Destructor)AstNode_freeWhole);
        Lexer_clearTokens(tokens);
        List_free(tokens);
       return status;
    } else {
        Lexer_clearTokens(tokens);
        List_free(tokens);
        Var * err = Var_new(Type_STRING);
        String_append(Var_stringAt(err), "Parser error");
        return err;    
    }
}




