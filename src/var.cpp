#include "var.h"
#include "error.h"
#include "var_double.h"
#include "var_boolean.h"
#include "var_list.h"
#include "var_string.h"
#include "var_object.h"
#include "var_function.h"
#include "var_integer.h"

namespace Calamity {

	Var::Var(const Var::Type &type) : m_type(type) {
		switch (m_type) {
			case Type::Null:
			case Type::Undefined: {
				m_value = nullptr;
				return;
			}
			case Type::List: {
				m_value = new List;
				return;
			}
			case Type::String: {
				m_value = new String;
				return;
			}
			case Type::Boolean: {
				m_value = new Boolean;
				return;
			}
			case Type::Number: {
				m_value = new Double;
				return;
			}
			default: {
				CaDebug_noimpl();
			}
		}
	}

	void Var::setNewValue() {
		CaDebug_noimpl();
	}

	Var::Var(const double &d)
			: m_type(Type::Number), m_value(new Double(d)) {}

	Var::Var(const Double &d)
			: m_type(Type::Number), m_value(new Double(d)) {}

	Var::Var(const bool &b)
			: m_type(Type::Boolean), m_value(new Boolean(b)) {}

	Var::Var(const Boolean &b)
			: m_type(Type::Boolean), m_value(new Boolean(b)) {}

	typedef ::Calamity::String String;

	Var::Var(String * const & string)
			: m_type(Var::Type::String), m_value(string) {}

	Var::Var(String && string)
			: m_type(Type::String),
			  m_value(new String(std::move(string))) {}

	Var::Var(List * const & list)
			: m_type(Type::List),
			  m_value(list) {}

	Var::Var(List &&list)
			: m_type(Type::List),
			  m_value(new List(std::move(list))) {}

	Var::~Var() {
		deleteValue();
	}

	Var::Var(Var &&rvalue)
			: m_value(rvalue.m_value),
			  m_type(rvalue.m_type) {
		rvalue.m_type = Type::Undefined;
		rvalue.m_value = nullptr;
	}

	Var::Var(const Var &lvalue) {
		switch (lvalue.type()) {
			case Type::Boolean:   { m_value = new Boolean(lvalue.boolean()); return; }
			case Type::Number:    { m_value = new Double(lvalue.number());   return; }
			case Type::Undefined: { m_value = nullptr;                       return; }
			case Type::String:    { new String(lvalue.string());             return; }
			case Type::List:      { new List(lvalue.list());                 return; }
			default: {
				CaDebug_noimpl();
			}
		}
	}

	Var Var::makeList() {
		return new List;
	}

	Var Var::makeString() {
		return new String;
	}

	Var Var::copy(const Var &toCopy) {
		switch (toCopy.type()) {
			case Type::Boolean: {
				return toCopy.boolean();
			}
			case Type::Number: {
				return toCopy.number();
			}
			case Type::Undefined: {
				return Var();
			}
			case Type::String: {
				return new String(toCopy.string());
			}
			case Type::List: {
				return new List(toCopy.list());
			}
			default: {
				CaDebug_noimpl();
			}
		}
	}

	bool Var::isNumber() const {
		return m_type == Type::Number;
	}

	bool Var::isBoolean() const {
		return m_type == Type::Boolean;
	}

	bool Var::isString() const {
		return m_type == Type::String;
	}

	bool Var::isUndefined() const {
		return m_type == Type::Undefined;
	}

	bool Var::isList() const {
		return m_type == Type::List;
	}

	bool Var::isNull() const {
		return m_type == Type::Null;
	}

	bool Var::isFunction() const {
		return m_type == Type::Function;
	}

	bool Var::isObject() const {
		return m_type == Type::Object;
	}

	Var::Type Var::type() const {
		return m_type;
	}


	std::wostream &operator<<(std::wostream &stream, const Var &self) {
		switch (self.type()) {
			case Var::Type::Undefined: {
				return stream << "undefined";
			}
			case Var::Type::Boolean: {
				return stream << self.boolean();
			}
			case Var::Type::Number: {
				return stream << self.number();
			}
			case Var::Type::String: {
				return stream << self.string();
			}
			case Var::Type::List: {
				return stream << self.list();
			}
			default: {
				CaDebug_noimpl();
			}
		}
	}

	String &Var::string() {
		CaDebug_suppose(isString());
		return *static_cast<String *>(m_value);
	}

	Double &Var::number() {
		CaDebug_suppose(isNumber());
		return *static_cast<Double *>(m_value);
	}

	Boolean &Var::boolean() {
		CaDebug_suppose(isBoolean());
		return *static_cast<Boolean *>(m_value);
	}

	List & Var::list() {
		CaDebug_suppose(isList());
		return *static_cast<List *>(m_value);
	}

	Object &Var::object() {
		CaDebug_suppose(isObject());
		return *static_cast<Object *>(m_value);
	}

	Function &Var::function() {
		CaDebug_suppose(isFunction());
		return *static_cast<Function *>(m_value);
	}

	const String &Var::string() const {
		CaDebug_suppose(isString());
		return *static_cast<String *>(m_value);
	}

	const Double &Var::number() const {
		CaDebug_suppose(isNumber());
		return *static_cast<Double *>(m_value);
	}

	const Boolean &Var::boolean() const {
		CaDebug_suppose(isBoolean());
		return *static_cast<Boolean *>(m_value);
	}

	const List & Var::list() const {
		CaDebug_suppose(isList());
		return *static_cast<List *>(m_value);
	}

	const Object &Var::object() const {
		CaDebug_suppose(isObject());
		return *static_cast<Object *>(m_value);
	}

	const Function &Var::function() const {
		CaDebug_suppose(isFunction());
		return *static_cast<Function *>(m_value);
	}

    bool Var::isNegative() const               { return number().isNegative();  }
	Integer64 Var::toInteger64() const         { return number().toInteger64(); }
	int64_t Var::to_int64() const              { return number().to_int64();    }
	const cachar_t * Var::typeName() const         { return typeName(m_type);       }

	const cachar_t * Var::typeName(const Type &type) {
		switch (type) {
			case Type::Undefined: return ca("undefined");
			case Type::Boolean:   return ca("boolean");
			case Type::Number:    return ca("number");
			case Type::String:    return ca("string");
			case Type::List:      return ca("list");
			case Type::Null:      return ca("null");
			case Type::Function:  return ca("function");
			case Type::Object:    return ca("object");
		}
		CaDebug_unreachable();
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
            case Type::Undefined: return undefined;
            case Type::Boolean:   return boolean;
            case Type::Number:    return number;
            case Type::String:    return string;
            case Type::List:      return list;
            case Type::Null:      return null;
            case Type::Function:  return function;
            case Type::Object:    return object;
        }
        CaDebug_unreachable();
    }


	void Var::deleteValue() {
		switch (m_type){
			case Type::Boolean:  { delete static_cast<Boolean *> (m_value); return; }
			case Type::Number:   { delete static_cast<Double *>  (m_value); return; }
			case Type::String:   { delete static_cast<String *>  (m_value); return; }
			case Type::List:     { delete static_cast<List *>    (m_value); return; }
			case Type::Function: { delete static_cast<Function *>(m_value); return; }
			case Type::Object:   { delete static_cast<Object *>  (m_value); return; }
			default : return; // case Type::Undefined or Type::Null, m_value == nullptr
		}
	}

	void Var::setNull() {
		deleteValue();
		m_type = Type::Null;
	}

	void Var::setUndefined() {
		deleteValue();
		m_type = Type::Undefined;
	}

	Var & Var::operator=(const Var & var) {
		switch (var.m_type){
			case Type::Undefined: { setUndefined(); return *this; }
			case Type::Null:      { setNull();      return *this; }
			case Type::String:    {
				return *this = String(var.string());
			}
			case Type::Function: {
				CaDebug_noimpl();
				return *this;
			}
			case Type::Object:{
				CaDebug_noimpl();
				return *this;
			}
			case Type::Number:{
				return *this = var.number();
			}
			case Type::List:{
				return *this = List(var.list());
			}
		}
		CaDebug_unreachable();
	}

	Var & Var::operator=(const Double & number) {
		return *this = number.value();
	}

	Var & Var::operator=(const Boolean & boolean) {
		return *this = boolean.value();
	}

	Var & Var::operator=(Var && rvar) {
		CaDebug_noimpl();
	}


	Var & Var::operator=(String && string) {
		if (isString()) {
			*static_cast<String *>(m_value) = std::move(string);
		} else {
			CaDebug_log(ca("Performing type switch from ") << typeName() << ca(" to string"));
			deleteValue();
			m_value = new String(std::move(string));
			m_type = Type::String;
		}
		return *this;
	}


	Var &Var::operator=(String *const &stringPtr) {
		CaDebug_suppose(stringPtr != m_value);
		deleteValue();
		m_type = Type::String;
		m_value = stringPtr;
		return *this;
	}

	Var &Var::operator=(List && list) {
		if (isList()) {
			*static_cast<List *>(m_value) = std::move(list);
		} else {
			CaDebug_log(ca("Performing type switch from ") << typeName() << ca(" to list"));
			deleteValue();
			m_value = new List(std::move(list));
			m_type = Type::List;
		}
		return *this;
	}

	Var &Var::operator=(List *const & listPtr) {
		CaDebug_suppose(listPtr != m_value);
		deleteValue();
		m_type = Type::List;
		m_value = listPtr;
		return *this;
	}

	Var &Var::operator=(const double &floatingPoint) {
		if (isNumber()) {
			*static_cast<Double *>(m_value) = floatingPoint;
		} else {
			CaDebug_log(ca("Performing type switch from ") << typeName() << ca(" to number"));
			deleteValue();
			m_value = new Double(floatingPoint);
			m_type = Type::Number;
		}
		return *this;
	}

	Var &Var::operator=(Double *const &doubleClass) {
		CaDebug_suppose(doubleClass != m_value);
		deleteValue();
		m_type = Type::Number;
		m_value = doubleClass;
		return *this;
	}

	Var &Var::operator=(const bool &boolean) {
		if (isBoolean()) {
			*static_cast<Boolean *>(m_value) = boolean;
		} else {
			CaDebug_log(ca("Performing type switch from ") << typeName() << ca(" to boolean"));
			deleteValue();
			m_value = new Boolean(boolean);
			m_type = Type::Boolean;
		}
		return *this;
	}

	Var &Var::operator=(Boolean * const & booleanClass) {
		CaDebug_suppose(booleanClass != m_value);
		deleteValue();
		m_type = Type::Boolean;
		m_value = booleanClass;
		return *this;
	}


	Exception Var::invalidBinaryOperand(const char *const &oper, Var &rightOperand) {
		String error(ca("invalid operands for binary operator ("));
		error += this->typeName();
		error += ca(' ');
		error += oper;
		error += ca(' ');
		error += rightOperand.typeName();
		error += ca(')');
		return Exception(std::move(error));
	}

	Var &Var::operator+=(Var &var) {
		if (var.isUndefined() || this->isUndefined()) {
			setUndefined();
			return *this;
		}
		CaDebug_noimpl();
	}

	Var &Var::assignAddRvalue(Var &var) {
		switch (m_type) {
			case Type::Undefined:
				return *this;
			case Type::Boolean: {
				return assignAddBooleanRvalue(var);
			}
			case Type::String: {
				return assignAddStringRvalue(var);
			}
			case Type::List: {
				CaDebug_noimpl();
//				return assignAddVectorRvalue(var);
			}
			case Type::Number: {
				return assignAddNumberRvalue(var);
			}
		}
		return *this;
	}

	Var &Var::assignAddLvalue(Var &var) {
		switch (m_type) {
			case Type::Undefined:
				return *this;
			case Type::Boolean: {
				return assignAddBooleanLvalue(var);
			}
			case Type::String: {
				return assignAddStringLvalue(var);
			}
			case Type::List: {
				CaDebug_noimpl();
//				return assignAddVectorLvalue(var);
			}
			case Type::Number: {
				CaDebug_noimpl();
//				return assignAddNumberLvalue(var);
			}
			default: {
				CaDebug_noimpl();
			}
		}
	}

	Var &Var::assignAddBooleanRvalue(Var &var) {
		throw invalidBinaryOperand("+=", var);
	}

	Var &Var::assignAddBooleanLvalue(Var &var) {
		throw invalidBinaryOperand("+=", var);
	}

	Var &Var::assignAddStringRvalue(Var &var) {
		CaDebug_noimpl();
//		var.appendNumberToString(this->string());
		return *this;
	}

	Var &Var::assignAddStringLvalue(Var &var) {
		CaDebug_noimpl();
//		var.appendNumberToString(this->string());
		return *this;
	}

//	Var &Var::assignAddVectorRvalue(Var &var) {
//		if (var.isList()) {
//			this->vector().reserve(vector().size() + var.vector().size());
//			for (Var &item : var.list()) {
//				vector().emplace_back(std::move(item));
//			}
//		} else {
//			vector().emplace_back(std::move(var));
//		}
//		return *this;
//	}

//	Var &Var::assignAddVectorLvalue(Var &var) {
//		if (var.isList()) {
//			this->vector().reserve(vector().size() + var.vector().size());
//			for (Var &item : var.list()) {
//				vector().emplace_back(copy(item));
//			}
//		} else {
//			vector().emplace_back(copy(var));
//		}
//		return *this;
//	}

	Var &Var::assignAddNumberRvalue(Var &var) {
		switch (var.type()) {
			case Type::Number: {
				number() += var.number();
				break;
			}
			case Type::List: {
				CaDebug_noimpl();
//				assignPrependToVector(var);
				break;
			}

		}
		return *this;
	}


	Var Var::operator==(Var &other) {
		CaDebug_noimpl();
//		if (other.m_type != m_type) {
//			return getRvalue(false, other);
//		} else if (this == &other) {
//			return getRvalue(true, other);
//		}
//		switch (m_type) {
//			case Type::Undefined:
//				return true;
//			case Type::String: {
//				return this->string() == other.string();
//			}
//			case Type::Boolean: {
//				return getRvalue(this->boolean() == other.boolean(), other);
//			}
//			case Type::Number: {
//				return this->number() == other.number();
//			}
//			case Type::List: {
//				if (this->list().size() != other.list().size()) {
//					return false;
//				} else {
//					auto selfIterator = this->list().begin();
//					auto otherIterator = other.list().begin();
//					auto selfEndIterator = this->list().end();
//					while (selfIterator != selfEndIterator) {
//						if (*selfIterator != *otherIterator) {
//							return false;
//						}
//						++selfIterator;
//						++otherIterator;
//					}
//					return true;
//				}
//			}
//			default: {
//				CaDebug_noimpl();
//			}
//		}
	}




}