#pragma once

#include <iostream>

#include "defs.h"
#include "var_integer.h"
#include "var_boolean.h"
#include "var_number.h"
#include "var_string.h"
#include "var_object.h"
#include "var_function.h"
#include "var_list.h"
#include "native_exception.h"
#include "vtem.h"

namespace Cala {

    class Var {
    public:
        enum class Type : unsigned char {
            Undefined,
            Number,
            String,
            Boolean,
            List,
            Null,
            Function,
            Object
        };

    private:
        union UValue {
            Boolean    boolean;
            Number     number;
            String   * string;
            List     * list;
            Function * function;
            Object   * object;

            meta_inline UValue() = default;
            meta_inline UValue(const size_t & i)        : number(i)   {}
            meta_inline UValue(const float_t & d)        : number(d)   {}
            meta_inline UValue(const Number & d)        : number(d)   {}
            meta_inline UValue(const bool & b)          : boolean(b)  {}
            meta_inline UValue(const Boolean & b)       : boolean(b)  {}
            meta_inline UValue(String * const & s)      : string(s)   {}
            meta_inline UValue(List * const & l)        : list(l)     {}
            meta_inline UValue(Function * const & f)    : function(f) {}
            meta_inline UValue(Object * const & o)      : object(o)   {}
        };
        Type m_type;
        UValue m_value;
    public:
        explicit Var(const Type & type);
        Var()                        : m_type(Type::Undefined)           {}
        Var(const Undefined &)       : m_type(Type::Undefined)           {}
        Var(const Null &)            : m_type(Type::Null)                {}
        Var(const float_t &d)         : m_type(Type::Number),  m_value(d) {}
        Var(const size_t &i)         : m_type(Type::Number),  m_value(i) {}
        Var(const Number &d)         : m_type(Type::Number),  m_value(d) {}
        Var(const bool &b)           : m_type(Type::Boolean), m_value(b) {}
        Var(const Boolean &b)        : m_type(Type::Boolean), m_value(b) {}
        Var(String * const & string) : m_type(Type::String),  m_value(string) {}
        Var(String && string)        : m_type(Type::String),  m_value(new String(std::move(string))) {}
        Var(List * const & list)     : m_type(Type::List),    m_value(list) {}
        Var(List &&list)             : m_type(Type::List),    m_value(new List(std::move(list))) {}

        ~Var() { deleteValue(); }
        Var(Var && rvalue) noexcept
            : m_type(rvalue.m_type), m_value(static_cast<UValue &&>(rvalue.m_value)){
            rvalue.m_type = Type::Undefined;
        }

        Var(const Var & lvalue);

        meta_inline bool isNumber() const    { return m_type == Type::Number;    }
        meta_inline bool isBoolean() const   { return m_type == Type::Boolean;   }
        meta_inline bool isString() const    { return m_type == Type::String;    }
        meta_inline bool isUndefined() const { return m_type == Type::Undefined; }
        meta_inline bool isList() const      { return m_type == Type::List;      }
        meta_inline bool isNull() const      { return m_type == Type::Null;      }
        meta_inline bool isFunction() const  { return m_type == Type::Function;  }
        meta_inline bool isObject() const    { return m_type == Type::Object;    }
        meta_inline bool isNullUndef() const {
            return m_type == Type::Null || m_type == Type::Undefined;
        }

        meta_inline bool operator==(const Undefined &) const { return m_type == Type::Undefined; }
        meta_inline bool operator==(const Null &) const      { return m_type == Type::Null;      }
        meta_inline bool operator!=(const Undefined &) const { return m_type != Type::Undefined; }
        meta_inline bool operator!=(const Null &) const      { return m_type != Type::Null;      }


        Var && rval() && = delete;
        Var && rval() const & = delete;
        meta_inline Var && rval() & { return static_cast<Var &&>(*this); }

        Type type() &&              { return m_type; }
        const Type & type() const & { return m_type; }

        friend ostream & operator<<(ostream & stream, const Var & self);

        Boolean &  boolean()  &;
        Number &   number()   &;
        String &   string()   &;
        List &     list()     &;
        Function & function() &;
        Object &   object()   &;

        const String &   string()   const &;
        const Number &   number()   const &;
        const Boolean &  boolean()  const &;
        const List &     list()     const &;
        const Object &   object()   const &;
        const Function & function() const &;
        Integer integer() const;

        bool isNegative() const;




        Var & operator=(Var && rvar) noexcept;
        Var & operator=(const Var & lvar);

        Var & operator=(String && string);
        Var & operator=(String * const & string);
        Var & operator=(const String & string);

        Var & operator=(List && list);
        Var & operator=(List * const & list);
        Var & operator=(const List & list);

        Var & operator=(const Number & number);
        Var & operator=(const float_t & floatingPoint);

        Var & operator=(const Boolean & boolean);
        Var & operator=(const bool & boolean);

        inline Var & operator=(const Null &) {
            deleteValue();
            m_type = Type::Null;
            return *this;
        }
        inline Var & operator=(const Undefined &){
            deleteValue();
            m_type = Type::Undefined;
            return *this;
        }
        Var & operator=(const cachar_t & character);

        Var & morphToPrimitive() &&;
        Var & morphToBoolean()   &&;
        Var & morphToNumber()    &&;
        Var & morphToString()    &&;

        Var     toPrimitive() &;
        Boolean toBoolean()   &;
        Number  toNumber()    &;
        String  toString()    &;


        Var & operator+=(const Var & lvalue);


        Var & operator-=(Var & var);
        Var & operator*=(Var & var);
        Var & operator/=(Var & var);




        Var operator+(Var & other);

        Var operator-(Var & other);

        Var operator*(Var & other);

        Var operator/(Var & other);

        Var operator&&(Var & other);

        Var operator||(Var & other);

        Var operator==(Var & other);

        Var operator!=(Var & other);

        Var operator>(Var & other);

        Var operator<(Var & other);

        Var operator>=(Var & other);

        Var operator<=(Var & other);

        Var operator[](Var & other);

        const cachar_t * typeName() const;
        static const cachar_t * typeName(const Type & type);

        static const String & typeString(const Type & type);
        const String & typeString() const;

        Var sqrt();

        Var pow(const Var & power);

        explicit operator bool() const;

        operator float_t() const;


    private:

        meta_inline String * stringPtr() {
            Debug_suppose(m_type == Type::String);
            return m_value.string;
        }
        meta_inline List * listPtr()     {
            Debug_suppose(m_type == Type::List);
            return m_value.list;
        }

        void deleteValue();

        Exception invalidBinaryOperand(const cachar_t * const & oper, const Var & operand);
    };


}