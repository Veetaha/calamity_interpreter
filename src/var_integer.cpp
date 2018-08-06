//
// Created by tegeran on 28/07/18.
//
#include "var.h"

#include "var_integer.h"


namespace Cala {

    template <>
    Integer Integer::cast(const Number & number){
        return static_cast<int_type>(number.value());
    }

    template <>
    Integer Integer::cast(const Boolean & boolean){
        return static_cast<int_type>(boolean.value());
    }

    list_iterator operator+(const list_iterator & iterator, const Integer & adding) {
        return iterator + adding.value();
    }

    list_iterator operator-(const list_iterator & iterator, const Integer & subtracting){
        return iterator - subtracting.value();
    }

    list_const_iterator operator+(const list_const_iterator & iterator, const Integer & adding){
        return iterator + adding.value();
    }

    list_const_iterator operator-(const list_const_iterator & iterator, const Integer & subtracting){
        return iterator - subtracting.value();
    }

    string_iterator operator+(const string_iterator & iterator, const Integer & adding){
        return iterator + adding.value();
    }

    string_iterator operator-(const string_iterator & iterator, const Integer & subtracting){
        return iterator - subtracting.value();
    }

    string_const_iterator operator+(const string_const_iterator & iterator, const Integer & adding) {
        return iterator + adding.value();
    }

    string_const_iterator operator-(const string_const_iterator & iterator, const Integer & subtracting){
        return iterator - subtracting.value();
    }



}