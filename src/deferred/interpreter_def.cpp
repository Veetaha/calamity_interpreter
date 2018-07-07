#include <interpreter_def.h>
#include <error.h>
#include <dynamictype.h>
#include <point2d.h>
#include <mem.h>

void MACRO_FREE_DYNTYPE_ARR(DynamicType ** arr, size_t size){
    Debug_exists(arr);
    //
    for (size_t i = 0; i < size - 1; ++i){
        if (arr[i]) DynamicType_free(arr[i]);
    }
}

void Calamity_setError(const char * msg, AstNode * node, Calamity * prog){
    Debug_suppose(prog->error == NULL, "An attempt to register error twice was made");
    Debug_suppose(prog->state != ProgState_ERROR, "An attempt to register error twice was made");
    //
    prog->state = ProgState_ERROR;
    prog->error = String_toNewString("Unresolved runtime error: ");
    String_append(prog->error, msg);
    Location2D errLoc = AstNode_locationAt(node);
    String_appendFormat(prog->error, " (%i : %i)\nLine %i:\n", errLoc.y, errLoc.x, errLoc.y);
    
    size_t lineStart = 0;
    if (errLoc.y > 0){
        lineStart = String_searchOccurence(prog->code, '\n', errLoc.y - 1) + 1;        
    }

    while (lineStart < String_length(prog->code) && String_at(prog->code, lineStart) != '\n'){
        String_appendChar(prog->error, String_at(prog->code, lineStart++));
    }

    String_appendChar(prog->error, '\n');
    for (unsigned int i = 0; i < errLoc.x - 1; ++i){
        String_appendChar(prog->error, ' ');
    }
    String_append(prog->error, "🠹\n");
}


Calamity * Calamity_new(Dict * vars, Dict * funcs, Dict * defs, String * error){
    Calamity * newbie = Mem_malloc(sizeof(Calamity));
    *newbie = (Calamity){
        .vars = vars,
        .funcs = funcs,
        .defs = defs,
        .error = error
    };
    return newbie;
}

void Calamity_free(Calamity * self){
    Debug_exists(self);
    //
    free(self);
}