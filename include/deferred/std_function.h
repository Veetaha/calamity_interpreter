#pragma once

#include <var.h>
#include <dict.h>
#include <interpreter_def.h>
#include <tree.h>

typedef Var * (*StdRoutine)(Calamity * prog, Tree * exec);

typedef struct __Function {
    StdRoutine f;
} Function;

Function * Function_new(StdRoutine func);
void Function_free(Function * self);
StdRoutine Function_at(Function * self);


Var * Calamity_setVarStorage(Calamity * prog, Tree * ast, Var * entity);
Var * Calamity_appendVarStorage(Calamity * prog, Tree * ast, Var * entity);

Var * std_getter   (Calamity * prog, Tree * name);
Var * std_ranger   (Calamity * prog, Tree * name);

Var * std_digify   (Calamity * prog, Tree * name);
Var * std_insert   (Calamity * prog, Tree * name);
Var * std_print    (Calamity * prog, Tree * name);
Var * std_write    (Calamity * prog, Tree * name);
Var * std_read     (Calamity * prog, Tree * name);
Var * std_remove   (Calamity * prog, Tree * name);
Var * std_sizeof   (Calamity * prog, Tree * name);
Var * std_sqrt     (Calamity * prog, Tree * name);
Var * std_stringify(Calamity * prog, Tree * name);
Var * std_typeof   (Calamity * prog, Tree * ast);
Var * std_escape   (Calamity * prog, Tree * ast);
Var * std_array    (Calamity * prog, Tree * ast);