//
// Created by tegeran on 08.07.18.
//

#include "native_exception.h"
#include "var_string.h"
#include "var.h"
#include "var_list.h"

namespace Calamity {
    template<>
    BasicString<cachar_t>
    BasicString<cachar_t>::readFromFile(const char * const & path) {
        std::basic_ifstream<cachar_t> file(path);
        if (! file.is_open()) {
            String errorString(ca("failed to open file \""));
            (errorString += path) += ca("\"");
            throw Exception(std::move(errorString));
        } else {
            std::basic_ostringstream<cachar_t> contentHolder;
            contentHolder << file.rdbuf();
            return contentHolder.str();
        }
    }

    String & operator+=(String & self, const char * cString) {
        self.reserveMore(std::char_traits<char>::length(cString));
        while (*cString){
            self += static_cast<cachar_t>(*cString++);
        }
        return self;
    }

    String & operator+=(String & self, const Var & var) {
        switch (var.type()) {
            case Var::Type::Undefined: {
                return self += ca("undefined");
            }
            case Var::Type::Boolean: {
                return self += var.boolean().toCaString();
            }
            case Var::Type::Number: {
                return self += var.number().toString();
            }
            case Var::Type::String: {
                return self += var.string();
            }
            case Var::Type::List: {        // average var.toString() size == 3
                self.reserve(self.size() + 3 * var.list().size());
                return std_ext::appendContainerToString(
                    self,
                    var.list(),
                    [](String & str, const Var & variable) {
                        str += variable;
                    }
                );
            }
            default: {
                CaDebug_noimpl();
            }
        }
    }
    String & operator>>(const Var & var, String & string) {
        switch (var.type()) {
            case Var::Type::Undefined: {
                return ca("undefined") >> string;
            }
            case Var::Type::Boolean: {
                return var.boolean().toCaString() >> string;
            }
            case Var::Type::Number: {
                return var.number().value() >> string;
            }
            case Var::Type::String: {
                return var.string() >> string;
            }
            case Var::Type::List: {               // average var.toString() size == 3
                string.reserve(string.size() + 3 * var.list().size());
                return std_ext::prependToString(
                    string,
                    var.list(),
                    [](String & str, const Var & variable) {
                        str += variable;
                    }
                );
            }
            default: {
                CaDebug_noimpl();
            }
        }
    }


}