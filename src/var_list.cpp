//
// Created by tegeran on 27.06.18.
//
#include <utility>
#include "var_list.h"
#include "var.h"
#include "var_number.h"
#include "var_string.h"



namespace Cala {

	int64_t List::size() const {
		return m_vector.size();
	}

	List::List(const List &lvalue)
		: m_vector(lvalue.m_vector) {}

	List::List(List && rvalue) noexcept
		: m_vector(std::move(rvalue.m_vector)){}

	List &List::operator=(const List &lvalue){
		m_vector = lvalue.m_vector;
		return *this;
	}

	List &List::operator=(List &&rvalue) noexcept{
		m_vector = std::move(rvalue.m_vector);
		return *this;
	}

	List &List::push_back(const List &lvalue) {
		m_vector.reserve(m_vector.size() + lvalue.size());
		m_vector.insert(m_vector.end(), lvalue.begin(), lvalue.end());
		return *this;
	}

	List &List::emplace_back(List && rvalue) {
		m_vector.reserve(m_vector.size() + rvalue.size());
		for (Var & rvar : rvalue){
			m_vector.emplace_back(static_cast<Var &&>(rvar));
		}
		return *this;
	}

	List &List::push_front(const List &lvalue) {
		m_vector.reserve(m_vector.size() + lvalue.size());
		m_vector.insert(m_vector.begin(), lvalue.begin(), lvalue.end());
		return *this;
	}

	List &List::emplace_front(List &&rvalue) {
		m_vector.reserve(m_vector.size() + rvalue.size());
		rvalue.emplace_back(std::move(*this));
		m_vector = std::move(rvalue.m_vector);
		return *this;
	}

	List &List::emplace_back(Var &&rvalue) {
		m_vector.emplace_back(std::move(rvalue));
		return *this;
	}

	List &List::push_back(const Var &lvalue) {
		m_vector.push_back(lvalue);
		return *this;
	}

	List &List::operator+=(const Var & lvalue) {
		return lvalue.isList()
			   ? push_back(lvalue.list())
			   : push_back(lvalue);
	}

	List &List::operator+=(Var &&rvalue) {
		return rvalue.isList()
				? emplace_back(std::move(rvalue.list()))
				: emplace_back(std::move(rvalue));
	}

	List &List::push_front(const Var &lvalue) {
		m_vector.insert(m_vector.begin(), lvalue);
		return *this;
	}

	List &List::emplace_front(Var &&rvalue) {
		m_vector.emplace(m_vector.begin(), std::move(rvalue));
		return *this;
	}

	List::operator bool() const {
		return !m_vector.empty();
	}

	Var &List::operator[](const Var &index) &{
        checkReadIndex(index);
		return m_vector[index.integer()];
	}

	Var &&List::operator[](const Var &index) &&{
		return static_cast<Var &&>(static_cast<List &>(*this).operator[](index));
	}


	void List::insert(const Var &index, Var &&rvalue) {
        checkInsertIndex(index);
		m_vector.emplace(m_vector.begin() + index.integer().value(), std::move(rvalue));
	}

	void List::insert(const Var &index, const Var &lvalue) {
        checkInsertIndex(index);
		m_vector.insert(m_vector.begin() + index.integer().value(), lvalue);
	}


	auto List::remove(const Var &index) {
        checkReadIndex(index);
		Var soldier(std::move(m_vector[index.integer().value()]));
		m_vector.erase(m_vector.begin() + index.integer().value());
		return soldier;
	}


	List operator+(const List & leftLval,  const List & rightLval){
        return List(leftLval) + rightLval;
	}
    List operator+(List &&      leftRval,       List && rightRval){
        return leftRval.emplace_back(std::move(rightRval));
    }
    List operator+(List &&      leftRval,  const List & rightLval){
        return leftRval.push_back(rightLval);
    }
    List operator+(const List & leftLval,       List && rightRval){
        return rightRval.push_front(leftLval);
    }




















	void List::checkReadIndex(const Var & index) const {
		if (!index.isNumber()){
			throw nonNumericListSubscriptException(index);

		} else if (index.integer() >= size()){
			throw indexOutOfBoundsException(index.number());

		} else if (index.isNegative()) {
			throw negativeIndexException(index.number());
		}
	}

	void List::checkInsertIndex(const Var & index) const {
		if (!index.isNumber()){
			throw nonNumericListSubscriptException(index);

		} else if (index.integer() > size()){
			throw indexOutOfBoundsException(index.number());

		} else if (index.isNegative()) {
			throw negativeIndexException(index.number());
		}
	}




	Exception List::indexOutOfBoundsException(const Number & index) const {
		String errorString;
		errorString.push_back(
            ca("list index is out of bounds [list.size = "), m_vector.size(),
            ca(", index = "), cast<Integer>(index).value(), ']'
        );
		return Exception(errorString.rval());
	}

	Exception List::nonNumericListSubscriptException(const Var &subscript) const {
		String errorString;
		errorString.push_back(
		    ca("list index subscript is of non-number type [typeof(subscript) = "),
		    subscript.typeName(), ca(']')
        );
		return Exception(errorString.rval());
	}

	Exception List::negativeIndexException(const Number &index) const {
		String errorString;
		errorString.push_back(
		    ca("list index subscript is negative [index = "),
		    cast<Integer>(index).value(), ca(']')
        );
		return Exception(errorString.rval());
	}

	Exception List::sublistInvalidRangeTypeException(const Var &begin, const Var &end) {
		String errorString;
		errorString.push_back(
		    ca("sublist range non-number argument type [typeof(begin) = "), begin.typeName(),
    		ca(", typeof(end) = "), end.typeName(), ca(']')
        );
		return Exception(errorString.rval());
	}

	Exception List::sublistRidiculousRangeExcetion(const Var &begin, const Var &end) {
		String errorString;
		errorString.push_back(
		    ca("sublist range is ridiculous [begin = "), begin.typeName(),
		    ca(", end = "), end.typeName(), ca(']')
        );
		return Exception(errorString.rval());
	}

	List List::sublist(const Var &begin, const Var &end) &{
        checkReadIndex(begin);
        checkReadIndex(end);
		if (begin.integer() > end.integer()){
			throw sublistRidiculousRangeExcetion(begin, end);
		} else {
			List sub;
			auto i(m_vector.begin()  + begin.integer());
			auto edge(m_vector.end() + end.integer());
			sub.reserve(static_cast<size_type>(edge - i));
			while (i != edge){
				sub.push_back(*i);
				++i;
			}
			return sub;
		}
	}

	List List::sublist(const Var &begin, const Var &end) &&{
        checkReadIndex(begin);
        checkReadIndex(end);
        Integer beginInt(begin.integer());
        Integer endInt(end.integer());
		if (beginInt > endInt){
			throw sublistRidiculousRangeExcetion(begin, end);
		} else {
			List sub;
			auto i(m_vector.begin()  + beginInt);
			auto edge(m_vector.end() + endInt);
			sub.m_vector.reserve(static_cast<size_t>(edge - i));
			while (i != edge){
				sub.emplace_back(i->rval());
				++i;
			}
			return sub;
		}
	}

	ostream & operator<<(ostream &stream, const List &self) {
		Vtem::print(self, stream);
        return stream;
	}

}


