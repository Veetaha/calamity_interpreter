//
// Created by tegeran on 27.06.18.
//

#include "var_list.h"
#include "var_double.h"
#include "var_string.h"
#include <utility>

namespace Calamity {

	int64_t List::size() const {
		return m_vector.size();
	}

	List::List(const List &lvalue)
		: m_vector(lvalue.m_vector) {}

	List::List(List && rvalue)
		: m_vector(std::move(rvalue.m_vector)){}

	List &List::operator=(const List &lvalue){
		m_vector = lvalue.m_vector;
		return *this;
	}

	List &List::operator=(List &&rvalue){
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

	bool List::to_bool() const {
		return !m_vector.empty();
	}

	Boolean List::toBoolean() const {
		return !m_vector.empty();
	}

	Var &List::operator[](const Var &index) &{
		checkReaderIndex(index);
		return m_vector[index.to_int64()];
	}

	Var &&List::operator[](const Var &index) &&{
		return std::move(static_cast<List &>(*this).operator[](index));
	}


	void List::insert(const Var &index, Var &&rvalue) {
		checkInserterIndex(index);
		m_vector.emplace(m_vector.begin() + index.to_int64(), std::move(rvalue));
	}

	void List::insert(const Var &index, const Var &lvalue) {
		checkInserterIndex(index);
		m_vector.insert(m_vector.begin() + index.to_int64(), lvalue);
	}


	Var List::remove(const Var &index) {
		checkReaderIndex(index);
		Var soldier(std::move(m_vector[index.to_int64()]));
		m_vector.erase(m_vector.begin() + index.to_int64());
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




















	void List::checkReaderIndex(const Var & index) const {
		if (!index.isNumber()){
			throw nonNumericListSubscriptException(index);

		} else if (index.to_int64() >= size()){
			throw indexOutOfBoundsException(index.number());

		} else if (index.isNegative()) {
			throw negativeIndexException(index.number());
		}
	}

	void List::checkInserterIndex(const Var &index) const {
		if (!index.isNumber()){
			throw nonNumericListSubscriptException(index);

		} else if (index.to_int64() > size()){
			throw indexOutOfBoundsException(index.number());

		} else if (index.isNegative()) {
			throw negativeIndexException(index.number());
		}
	}




	Exception List::indexOutOfBoundsException(const Double & index) const {
        CaDebug_noimpl();
//		String errorString("list index is out of bounds [list.size=");
//		errorString << m_vector.size();
//		errorString += ",index=";
//		errorString << index.to_int64();
//		errorString += ']';
//		return MessageException(std::move(errorString.string()));
	}

	Exception List::nonNumericListSubscriptException(const Var &subscript) const {
        CaDebug_noimpl();
//		String errorString("list index subscript is of non-number type [subscript.typename=");
//		errorString += subscript.typeName();
//		errorString += ']';
//		return MessageException(std::move(errorString.string()));
	}

	Exception List::negativeIndexException(const Double &index) const {
        CaDebug_noimpl();
//		String errorString("list index subscript is negative [subscript=");
//		errorString << index.to_int64();
//		errorString += ']';
//		return MessageException(std::move(errorString.string()));
	}

	Exception List::sublistInvalidRangeTypeException(const Var &begin, const Var &end) {
	    CaDebug_noimpl();
//		String errorString("sublist range non-number argument type [begin.typename=");
//		errorString += begin.typeName();
//		errorString += ", end.typename=";
//		errorString += end.typeName();
//		errorString += ']';
//		return MessageException(std::move(errorString.string()));
	}

	Exception List::sublistRidiculousRangeExcetion(const Var &begin, const Var &end) {
		String errorString(ca("sublist range is ridiculous [begin="));
		errorString += begin.typeName();
		errorString += ", end=";
		errorString += end.typeName();
		errorString += ']';
		return Exception(std::move(errorString));
	}

	List List::sublist(const Var &begin, const Var &end) &{
		checkReaderIndex(begin);
		checkReaderIndex(end);
		if (begin.to_int64() > end.to_int64()){
			throw sublistRidiculousRangeExcetion(begin, end);
		} else {
			List sub;
			auto i(m_vector.begin()  + begin.to_int64());
			auto edge(m_vector.end() + end.to_int64());
			sub.m_vector.reserve(static_cast<size_t>(edge - i));
			while (i != edge){
				sub.push_back(*i);
				++i;
			}
			return sub;
		}
	}

	List List::sublist(const Var &begin, const Var &end) &&{
		checkReaderIndex(begin);
		checkReaderIndex(end);
		if (begin.to_int64() > end.to_int64()){
			throw sublistRidiculousRangeExcetion(begin, end);
		} else {
			List sub;
			auto i(m_vector.begin()  + begin.to_int64());
			auto edge(m_vector.end() + end.to_int64());
			sub.m_vector.reserve(static_cast<size_t>(edge - i));
			while (i != edge){
				sub.emplace_back(std::move(*i));
				++i;
			}
			return sub;
		}
	}

	std::wostream & operator<<(std::wostream &stream, const List &self) {
		std_ext::print(self, stream);
        return stream;
	}
}


