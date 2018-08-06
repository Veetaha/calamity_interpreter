#include "native_exception.h"
#include "var_string.h"
#include "var_undefined.h"
#include "var_null.h"
#include "var.h"
#include "var_list.h"

namespace Cala {
    
    String String::readFromFile(const char * const & path) {
        ifstream file(path);
        if (!file.is_open()) {
            String errorString;
            errorString += ca("failed to open file \"");
            errorString += path;
            errorString += ca('\"');
            throw Exception(std::move(errorString));
        } else {
            ostringstream contentHolder;
            contentHolder << file.rdbuf();
            return contentHolder.str();
        }
    }

    String & String::operator+=(const char * cString) {
        reserveMore(Vtem::cstrlen(cString));
        while (*cString) {
            *this += static_cast<cachar_t>(*cString++);
        }
        return *this;
    }

    String & String::operator+=(const Var & var) {
        typedef Var::Type Type;

        switch (var.type()) {
            case Type::Null: {
                return *this += Cala::cast<String>(null);
            }
            case Type::Undefined: {
                return *this += Cala::cast<String>(undefined);
            }
            case Type::Boolean: {
                return *this += Cala::cast<String>(var.boolean());
            }
            case Type::Number: {
                return *this << *var.number();
            }
            case Type::String: {
                return *this += var.string();
            }
            case Type::List: {        // average var.toString() size == 3
                reserveMore(3 * var.list().size());
                return Vtem::appendContainerToString(
                    *this,
                    var.list(),
                    [](String & str, const Var & variable) {
                        str += variable;
                    }
                );
            }
            default: {
                Debug_noimpl();
            }
        }
    }

    String & operator>>(const Var & var, String & string) {
        typedef Var::Type Type;
        switch (var.type()) {
            case Type::Null: {
                return cast<String>(null) >> string;
            }
            case Type::Undefined: {
                return cast<String>(undefined) >> string;
            }
            case Type::Boolean: {
                return cast<String>(var.boolean()) >> string;
            }
            case Type::Number: {
                return var.number().value() >> string;
            }
            case Type::String: {
                return var.string() >> string;
            }
            case Type::List: {               // average var.toString() size == 3
                string.reserveMore(3 * var.list().size());
                return Vtem::prependToString(
                    string,
                    var.list(),
                    [](String & str, const Var & variable) {
                        str += variable;
                    }
                );
            }
            default: {
                Debug_noimpl();
            }
        }
    }

    
    String::String(const Var & var) {
        *this += var;
    }

    
    const String & String::cast(const Boolean & boolean){
        static String trueString  = ca("true");
        static String falseString = ca("false");
        return boolean ? trueString : falseString;
    }

    
    const String & String::cast(const Undefined &){
        static String undefinedString = ca("undefined");
        return undefinedString;
    }

    
    const String & String::cast(const Null &){
        static String nullString = ca("null");
        return nullString;
    }

    
    String String::cast(const List & lval){
        String str;
        return Vtem::appendContainerToString(str, lval);
    }

    
    String String::cast(List && rval){
        if (rval.isEmpty()) {
            return ca("[]");
        }
        Var & front(rval.front());
        if (front.isString()){
            return Vtem::appendContainerToString(front.string(), rval);
        } else {
            String str;
            return Vtem::appendContainerToString(str, rval);
        }
    }

}