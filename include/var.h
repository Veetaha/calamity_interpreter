#pragma once

#include <iostream>
#include "std_ext.h"
#include "var_integer.h"
#include "var_double.h"
#include "var_string.h"
#include "var_object.h"
#include "var_function.h"
#include "native_exception.h"


namespace Calamity {

    class List;

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
        void * m_value;
        Type m_type;
    public:
        Var(const Type & type = Type::Undefined);

        Var(const double & d);

        Var(const bool & b);

        Var(const Double & d);

        Var(const Boolean & d);

        Var(String * const & string);

        Var(List * const & list);

        Var(String && string);

        Var(List && list);

        ~Var();

        Var(Var && rvalue);

        Var(const Var & lvalue);

        static Var makeList();

        static Var makeString();

        static Var copy(const Var & toCopy);

        bool isNumber() const;

        bool isBoolean() const;

        bool isString() const;

        bool isList() const;

        bool isFunction() const;

        bool isObject() const;

        bool isUndefined() const;

        bool isNull() const;


        Type type() const;

        friend std::wostream & operator<<(std::wostream & stream, const Var & self);

        String & string();

        Double & number();

        Boolean & boolean();

        List & list();

        Object & object();

        Function & function();

        const String & string() const;

        const Double & number() const;

        const Boolean & boolean() const;

        const List & list() const;

        const Object & object() const;

        const Function & function() const;


        bool isNegative() const;

        int64_t to_int64() const;

        Integer64 toInteger64() const;


        inline static const Var & null() {
            static Var nil(Type::Null);
            return nil;
        };

        Var & operator=(Var && rvar);

        Var & operator=(const Var & lvar);

        Var & operator=(String && string);

        Var & operator=(String * const & string);

        Var & operator=(List && list);

        Var & operator=(List * const & list);

        Var & operator=(const Double & number);

        Var & operator=(const double & floatingPoint);

        Var & operator=(Double * const & doubleClass);

        Var & operator=(const Boolean & boolean);

        Var & operator=(const bool & boolean);

        Var & operator=(Boolean * const & booleanClass);


        Var & operator+=(Var & var);

        Var & operator-=(Var & var);

        Var & operator*=(Var & var);

        Var & operator/=(Var & var);


        void setUndefined();

        void setNull();


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

        operator double() const;

        void morphToString();

        Var & assignAddRvalue(Var & var);

        Var & assignAddLvalue(Var & var);

        Var & assignAddBooleanRvalue(Var & var);

        Var & assignAddStringRvalue(Var & var);

        Var & assignAddListRvalue(Var & var);

        Var & assignAddNumberRvalue(Var & var);

        Var & assignAddBooleanLvalue(Var & var);

        Var & assignAddStringLvalue(Var & var);

        Var & assignAddListLvalue(Var & var);

        Var & assignAddNumberLvalue(Var & var);


    private:
        void deleteValue();

        void setNewValue();

        Exception invalidBinaryOperand(const char * const & oper, Var & operand);

        void setLvalue(bool boolean);

        Var & assignPrependToList(Var & var);

        Var getRvalue(const bool & boolean, Var & useOther);

        Var getRvalue(const double & number, Var & useOther);
    };


}