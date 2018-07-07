/*
 * Copyright (C) Vitaly Kryvenko 2018.
 * e-mail:   gerzoh1@gmail.com
 * Telegram: @Simpliest_One
 * 
 * Special benedictions to my teacher Ruslan Gadyniak
 */ 

Calamity dynamically typed language is a simple, 
but effective tool to write small console programms.

All heap blocks are always freed, no leaks are possible.

Notes: 
    -numeric types: character, number
    -ast is built so that leftmost operators with the highest precedence are 
        set as leafs at the lowest level of the tree, thus 25/5/5 == 1 but not 25
    -previously if/else statement was called case/otherwise
    -endl token is just an alias for '\n'

Syntax (grammar.ebnf if you want to vkurit' totally):

COMMENTS:
# single line comment is begun by sharp
~ multi line comments are begun 
and terminated by til'da ~

FUNCTIONS:
1. All functions are declared before statements at the beginning of the program.
2. Functions are declared by the keyword 'define', function id, argument list in
    parentheses after it, statement (or block statement) which is a body of this function.
3. Functions use only local variables (argumens and locally declared variables).
4. Functions are free to call any other functions even declared after theirselves and
    and even themselves (recursively).
5. Declaring functions in functions is prohibited (this is compile time error).
6. User defined functions have fixed amount of arguments (function may have no arguments)
    and calling them with different number of arguments will cause a runtime error.
7. Function may return some value or nothing (in this case undefined is returned).
    'return' keyword and compulsory value after it provides this ability.
    (If you need to terminate program in-place, see information about escape() function bellow)
8. Return statent may be used in main code to return some value for external users.
    If it is not provided, then NULL is returned.

STANDART FUNCTIONS LIBRARY:

>> digify(entity)
    <-- returns: 
    switch ([entity]){
        case number:    number
        case character: character ASCII code
        case string:    array of numbers met in this string
        case array:     undefined
        case boolean:   1 if true, 0 if false
        case undefined: undefined
    }
>> insert(dest, index, entity) throws <Index exeptions>
    inserts [entity] to [dest] at index [index] 
    dest may be a variable identifier (its contents will be changed)
    <-- returs: dest after insertion
>> print(entity, ...)
    accepts arbitrary number of arguments (minimum 1) and outputs their values to console
    all values are automatically strigified in process
>> read() or read(path)
    read with no arguments reads a single line string from stdin (\n is thrown off)
    read(path) reads file from [path]
    <-- returns: string or undefined in case of failure
>> remove(dest, index)
    if dest is a single primitive type value and [index] == 0, then its value becomes undefined
    removes entity from [dest] at index [index] 
    dest may be a variable identifier (its content will be changed)
    <-- returs: dest after removal
>> sizeof(entity)
    <-- returns: 
    switch ([entity]){
        case number:    1
        case character: 1
        case string:    string's length
        case array:      array's size
        case boolean:   1
        case undefined: 1
    }
>> sqrt(numeric) throws <Non numeric value exception> <Sqrt for negative number exception>
    accepts only non-negtive numberic type argument
    <-- returns: mathematical square root of its argument

>> stringify(entity, ...)
    <-- returns: string which consists of all its arguments been written to a single string
    values are stringifed:
    switch([entity]){
        case number:    written like a sequence of ASCII digits with optional '-' sign
        case character: character as it is but in a string
        case string:    the same string is returned
        case array:     all values are written and a whitespace (' ') is provided inbetween
        case boolean:   word "true" if it is true, word "false" if it is false
        case undefined: word "undefined"
    }
>> typeof(entity)
    <-- returns: string with type of [entity] written in it
    possible returns:
    switch([entity]){
        case number:    "number"
        case character: "character"
        case string:    "string"
        case array:     "array"
        case boolean:   "boolean"
        case undefined: "undefined"
    }
>> escape(status)
    Terminates program workflow at any level of function stack and sets [status] as
    a global (main) return value
>> array(size)
    <-- returns: a new array instance initialized by undefined with size of [size]
>> write(filepath, entity, ...)
    accepts arbitrary number of arguments and writes their values to [filepath]
    all values are automatically strigified before writing process
    function accepts minimum 2 arguments: filepath and at least one entity


STATEMENTS:
0. All statements (excluding block statements) must be terminated by semicolon ';'
1. Variable declaration is treated like a statement.
2. Variables are declared by the keyword 'new' and variable id 
    (it may begin only be a letter or underscore('_'), fucntions also).
3. Variables are possible to be initialized right after declaration by '=' and expression.
4. Variables have block scopes. If variable was declared in a block or in a construction, or
    as a function, then it is destroyed after executing all statements in this construction,
    block or function.
5. All variables and functions have common namespace, 
    so no variable can't have name 'print' or 'digify'.
6. Expressions are a valid statements.
7. Single semicolon ';' results in a blank statement, which executes nothing
8. Return statement terminates local worklow and pops function from the stack.
9. If return is located in main code, then whole program is terminated and value
    after return is returned is program status.

CONSTRUCTIONS:
    'while' (condition) statement or block statement
    'if' (condition) statement_if_true, optional else statement_if_false

OPERATORS:
    NOTE: all arithmetic and logical operators except == and != return undefined value 
    if one of their operands is undefined

    numeric: +, -, *, /, % (remainder)
    logical: ==, !=, &&, ||, >, >=, <, <=
    unary: +, -, ! (not)
    memory: = (assign), ++ (concatenate), ++= (concatenate and assign)

    'arr ++= entity' is more preferably to use if you need to add an entity 
    to the and of array or string than just 'arr = arr ++ entity;'

    arr[i] (get value at i), arr[i ... j] (get subarray from i to j both inclusive)
    arr[i][j][k]... for multidimensional arrays
    getter and ranger may be used interchangebly: arr[i][k ... g][p ... q][b] ...
    assigning is applicable for getter" arr[i][j]... = entity;
    rangers are restricted to use for assigning (arr[i ... j] = entity)
    getters and rangers are only applicable for arrays and strings
    
TYPES:
    "number"    is an accuracy single precision floating point number

    "character" is a single byte ASCII character, its literal must be surrounded by single quotes '
        all standart ASCII escape sequences (except '\e') are possible to use: 
        '\n' '\t' '\\' '\'' '\"' '\b' '\v'  '\r' '\f' '\a' '\0'    

    "string"    is an array of characters ('\0' is impossible to access, but possible to set)
        its literal must be surrounded by double quotes " , escape sequenses are allso legible

    "array"     is a dynamic array of dynamic lentgh, with entities of arbitrary type
        compund (array) literals must be surrounded by braces '[' and ']', all entities
        must be delimited by a coma inside them, empty literal is posssible ('[]')

    "boolean"   is a single byte logical value having only two possible options: true or false

    "undefined" is a type with single value - undefined, it is common for unitinialized values      
                    or for functions wich are supposed actually to return "nothing"