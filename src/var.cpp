#include "var.h"
#include "defs.h"

#include "var_number.h"
#include "var_boolean.h"
#include "var_list.h"
#include "var_string.h"
#include "var_object.h"
#include "var_function.h"
#include "var_integer.h"
#include <memory>

namespace Cala {

	Var::Var(const Var::Type &type)
		: m_type(type) {
		switch (m_type) {
			case Type::Null:
			case Type::Undefined:{ return; }// nothing to initialize
			case Type::Boolean:  { m_value.boolean = false;      return; }
			case Type::Number:   { m_value.number  = 0;          return; }
			case Type::String:   { m_value.string  = new String; return; }
			case Type::List:     { m_value.list    = new List;   return; }
			default: {
				Debug_noimpl();
			}
		}
	}

	Var::Var(const Var & lvalue)
		: m_type(lvalue.m_type) {
		switch (lvalue.m_type) {
			case Type::Null:
			case Type::Undefined: { return; } // no value is needed
			case Type::Boolean:   { m_value.boolean = lvalue.boolean();  return; }
			case Type::Number:    { m_value.number  = lvalue.number();   return; }
			case Type::String:    { m_value.string = new String(lvalue.string()); return; }
			case Type::List:      { m_value.list   = new List(lvalue.list());     return; }
			default: {
				Debug_noimpl();
			}
		}
	}

	ostream &operator<<(ostream &stream, const Var &self) {
		switch (self.m_type) {
			case Var::Type::Null:      { return stream << "null";         }
			case Var::Type::Undefined: { return stream << "undefined";    }
			case Var::Type::Boolean:   { return stream << self.boolean(); }
			case Var::Type::Number:    { return stream << self.number();  }
			case Var::Type::String:    { return stream << self.string();  }
			case Var::Type::List:      { return stream << self.list();    }
			default: {
				Debug_noimpl();
			}
		}
	}

	String &Var::string() & {
		Debug_suppose(isString());
		return *m_value.string;
	}

	Number &Var::number() & {
		Debug_suppose(isNumber());
		return m_value.number;
	}

	Boolean &Var::boolean() & {
		Debug_suppose(isBoolean());
		return m_value.boolean;
	}

	List & Var::list() & {
		Debug_suppose(isList());
		return *m_value.list;
	}

	Object &Var::object() & {
		Debug_suppose(isObject());
		return *m_value.object;
	}

	Function &Var::function() & {
		Debug_suppose(isFunction());
		return *m_value.function;
	}

	const String &Var::string() const & {
		Debug_suppose(isString());
		return *m_value.string;
	}

	const Number &Var::number() const & {
		Debug_suppose(isNumber());
		return m_value.number;
	}

	const Boolean &Var::boolean() const & {
		Debug_suppose(isBoolean());
		return m_value.boolean;
	}

	const List & Var::list() const & {
		Debug_suppose(isList());
		return *m_value.list;
	}

	const Object &Var::object() const & {
		Debug_suppose(isObject());
		return *m_value.object;
	}

	const Function &Var::function() const & {
		Debug_suppose(isFunction());
		return *m_value.function;
	}

	Integer Var::integer() const {
		Debug_suppose(isNumber());
		return cast<Integer>(number());
	}


    bool Var::isNegative() const               { return number().isNegative();  }
	const cachar_t * Var::typeName() const     { return typeName(m_type);       }

	const cachar_t * Var::typeName(const Type &type) {
		switch (type) {
			case Type::Null:      return ca("null");
			case Type::Undefined: return ca("undefined");
			case Type::Boolean:   return ca("boolean");
			case Type::Number:    return ca("number");
			case Type::String:    return ca("string");
			case Type::List:      return ca("list");
			case Type::Function:  return ca("function");
			case Type::Object:    return ca("object");
		}
		Debug_unreachable();
	}

	const String & Var::typeString() const {
	    return typeString(m_type);
	}

    const String & Var::typeString(const Var::Type & type){
        static String undefined = ca("undefined");
        static String boolean   = ca("boolean");
        static String number    = ca("number");
        static String string    = ca("string");
        static String list      = ca("list");
        static String null      = ca("null");
        static String function  = ca("function");
        static String object    = ca("object");
        switch (type) {
			case Type::Null:      return null;
            case Type::Undefined: return undefined;
            case Type::Boolean:   return boolean;
            case Type::Number:    return number;
            case Type::String:    return string;
            case Type::List:      return list;
            case Type::Function:  return function;
            case Type::Object:    return object;
        }
        Debug_unreachable();
    }


	void Var::deleteValue() {
		switch (m_type){
			case Type::String:   { delete m_value.string;   return; }
			case Type::List:     { delete m_value.list;     return; }
			case Type::Function: { delete m_value.function; return; }
			case Type::Object:   { delete m_value.object;   return; }
			default : return; // nothing to delete or call destructor for non-pointer types
		}
	}



	Var & Var::operator=(const Var & var) {
		if (this == &var) { return *this; } // self assignment
		switch (var.m_type) {
			case Type::Null:      { return *this = null;          }
			case Type::Undefined: { return *this = undefined;     }
			case Type::Boolean:   { return *this = var.boolean(); }
			case Type::Number:    { return *this = var.number();  }
			case Type::String:    { return *this = var.string();  }
			case Type::List:      { return *this = var.list();    }
			case Type::Function: {
				Debug_noimpl();
				return *this;
			}
			case Type::Object:{
				Debug_noimpl();
				return *this;
			}
		}
		Debug_unreachable();
	}

	Var & Var::operator=(Var && rvar) noexcept {
		Debug_suppose(&rvar != this); // no self assignment for rvalues
		switch (rvar.m_type){
			case Type::Null:      { return *this = null;      }
			case Type::Undefined: { return *this = undefined; }
			case Type::Boolean:   { return *this = rvar.boolean(); }
			case Type::Number:    { return *this = rvar.number();  }
			case Type::String:    { *this = rvar.stringPtr(); break; }
			case Type::List:      { *this = rvar.listPtr();   break; }
			case Type::Function:  { Debug_noimpl(); break; }
			case Type::Object:    { Debug_noimpl(); break; }
		}
		rvar.m_type = Type::Undefined;
		return *this;
	}

	Var & Var::operator=(const Number & number) {
		if (!this->isNumber()) {
			deleteValue();
			m_type = Type::Number;
		}
		m_value.number = number;
		return *this;
	}

	Var & Var::operator=(const Boolean & boolean) {
		if (!this->isBoolean()) {
			deleteValue();
			m_type = Type::Boolean;
		}
		m_value.boolean = boolean;
		return *this;
	}

	Var & Var::operator=(String && string) {
		if (this->isString()) {
			*m_value.string = std::move(string);
		} else {
			Debug_log(ca("variable changed type from ") << typeName() << ca(" to string"));
			deleteValue();
			m_type = Type::String;
			m_value.string = new String(std::move(string));
		}
		return *this;
	}

	Var & Var::operator=(List && list) {
		if (this->isList()) {
			*m_value.list = std::move(list);
		} else {
			Debug_log(ca("variable changed type from ") << typeName() << ca(" to list"));
			deleteValue();
			m_type = Type::List;
			m_value.list = new List(std::move(list));
		}
		return *this;
	}

	Var & Var::operator=(const cachar_t & character){
		if (this->isString()) {
			*m_value.string = character;
		} else {
			Debug_log(ca("variable changed type from ") << typeName() << ca(" to string"));
			deleteValue();
			m_type = Type::String;
			m_value.string = new String(character);
		}
		return *this;
	}

	Var & Var::operator=(const String & string) {
		if (this->isString()) {
			*m_value.string = string;
		} else {
			Debug_log(ca("variable changed type from ") << typeName() << ca(" to string"));
			deleteValue();
			m_type = Type::String;
			m_value.string = new String(string);
		}
		return *this;
	}

	Var & Var::operator=(const List & list) {
		if (this->isList()) {
			*m_value.list = list;
		} else {
			Debug_log(ca("variable changed type from ") << typeName() << ca(" to list"));
			deleteValue();
			m_type = Type::List;
			m_value.list = new List(list);
		}
		return *this;
	}


	Var &Var::operator=(String * const &stringPtr) {
		// debug check for self assignment
		Debug_supposeIf(isString(), m_value.string != stringPtr);
		deleteValue();
		m_type = Type::String;
		m_value.string = stringPtr;
		return *this;
	}

	Var &Var::operator=(List * const & listPtr) {
		// debug check for self assignment
		Debug_supposeIf(isList(), m_value.list != listPtr);
		deleteValue();
		m_type = Type::List;
		m_value.list = listPtr;
		return *this;
	}

	Var &Var::operator=(const float_t & floatingPoint) {
		if (!this->isNumber()){
			deleteValue();
			m_type = Type::Number;
		}
		m_value.number = floatingPoint;
		return *this;
	}


	Var &Var::operator=(const bool & boolean) {
		if (!this->isBoolean()) {
			deleteValue();
			m_type = Type::Boolean;
		}
		m_value.boolean = boolean;
		return *this;
	}

	Var &Var::operator+=(const Var &) { // unused parameter 'lvalue' whilst unimplemeted

		Debug_noimpl();
	}



    Var & Var::morphToPrimitive() && {
        switch (m_type) {
            default: { return *this; } // non-primitive are functions and objects and lists
            case Type::List:      { return *this = cast<String>(list()); }
            case Type::Function:  { Debug_noimpl(); }
            case Type::Object:    { Debug_noimpl(); }
        }
        return *this;
    }


    Var & Var::morphToBoolean() && {
        switch (m_type) {
            case Type::Null:
            case Type::Undefined: { *this = false;                                     break; }
            case Type::Boolean:   {                                                    break; }
            case Type::Number:    { *this = cast<Boolean>(number());                   break; }
            case Type::String:    { *this = cast<Boolean>(string());                   break; }
            case Type::List:
            case Type::Function:
            case Type::Object:    { *this = true;                                      break; }
        }
        return *this;
    }

    Var & Var::morphToNumber() && {
        switch (m_type) {
            case Type::Null:      { *this = cast<Number>(null);                     break; }
            case Type::Undefined: { *this = cast<Number>(undefined);                break; }
            case Type::Boolean:   { *this = cast<Number>(boolean());                break; }
            case Type::Number:    {                                                 break; }
            case Type::String:    { *this = cast<Number>(string());                 break; }
            case Type::List:
            case Type::Function:
            case Type::Object:    { *this = Number::nan;                            break; }
        }
        return *this;
    }











    Exception Var::invalidBinaryOperand(const cachar_t * const &oper, const Var & rightOperand) {
        String error;
        error.push_back(
            ca("invalid operands for binary operator ["),
            this->typeName(), ca(' '), oper, ca(' '), rightOperand.typeName(), ca(']')
        );
        return Exception(error.rval());
    }



}
