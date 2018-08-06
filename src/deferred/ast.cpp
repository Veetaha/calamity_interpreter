#include <ast.h>
#include <error.h>
#include <wchar.h>

namespace Cala {


    AstNodeType AstNode_typeAt(AstNode *self) {
        Debug_exists(self);
        //
        return self->type;
    }


    Var *AstNode_valAt(AstNode *self) {
        Debug_exists(self);
        //
        return self->val;
    }

    Location AstNode_locationAt(AstNode *self) {
        Debug_exists(self);
        //
        return self->loc;
    }


    AstNodeType AstNode_setType(AstNode *self, AstNodeType type) {
        Debug_exists(self);
        //
        AstNodeType prev = self->type;
        self->type = type;
        return prev;
    }

    Var *AstNode_setVal(AstNode *self, Var *val) {
        Debug_exists(self);
        //
        Var *prev = self->val;
        self->val = val;
        return prev;
    }

    Location AstNode_setLocation(AstNode *self, Location loc) {
        Debug_exists(self);
        //
        Location prev = self->loc;
        self->loc = loc;
        return prev;
    }


    AstNode *AstNode_new(AstNodeType type, Var *val, Location loc, String *name) {
        AstNode *newbie = Mem_malloc(sizeof(AstNode));
        *newbie = (AstNode) {
                .type = type,
                .val = val,
                .loc = loc,
                .name = name
        };
        return newbie;
    }


    void AstNode_free(AstNode *self) {
        Debug_exists(self);
        //
        free(self);
    }

    void AstNode_freeWhole(AstNode *self) {
        Debug_exists(self);
        if (self->val != NULL) {
            Debug_suppose(self->type != AstNodeType_IDENTIFIER, "Identifier with dynamic type is deteced");
            Var_free(self->val);
        } else if (self->name != NULL)
            String_free(self->name);
        free(self);
    }

    void AstNode_printWide(AstNode *self) {
        AstNodeType_printWide(self->type);
        if (self->type == AstNodeType_IDENTIFIER) {
            putwchar(' ');
            String_printWide(self->name);
            return;
        } else if (self->val != NULL) {
            wprintf(L", ");
            Var_printWide(self->val);
        }
    }

    void AstNodeType_printWide(AstNodeType type) {
        for (int i = 0; AST_TYPE_NAMES[type][i] != '\0'; ++i) {
            putwchar(AST_TYPE_NAMES[type][i]);
        }
    }


    String *AstNode_setName(AstNode *self, String *name) {
        Debug_exists(self);
        //
        String *prev = self->name;
        self->name = name;
        return prev;
    }

    String *AstNode_nameAt(AstNode *self) {
        Debug_exists(self);
        //
        return self->name;
    }

    const char *AstNodeType_toConstChars(AstNodeType type) {
        return AST_TYPE_NAMES[type];
    }

    void AstNode_appendToString(String *str, AstNode *self) {
        Debug_exists(self);
        Debug_exists(str);
        //
        String_append(str, AstNodeType_toConstChars(self->type));
        if (self->type == AstNodeType_IDENTIFIER) {
            String_appendChar(str, ' ');
            String_concatenate(str, self->name);
            return;
        } else if (self->val != NULL) {
            String_append(str, ", ");
            switch (Var_typeAt(self->val)) {
                case Type_STRING: {
                    String_appendChar(str, '\"');
                    Var_appendToString(str, self->val);
                    String_appendChar(str, '\"');
                    return;
                }
                case Type_CHARACTER: {
                    String_appendChar(str, '\'');
                    Var_appendToString(str, self->val);
                    String_appendChar(str, '\'');
                    return;
                }
                default: {
                    Var_appendToString(str, self->val);
                    return;
                }
            }
        }
    }

}