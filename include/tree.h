#pragma once

#include "std_ext.h"
#include <vector>
#include <utility>
#include <algorithm>
#include "defs.h"
template <typename T>
class Tree {
	T m_val;
	std::vector<Tree<T>> m_children;
public:
	DECL_DEFAULT_MOVE_ONLY(Tree);


	explicit Tree(T &&initialValue = T());

	void setChild(const size_t &index, Tree<T> && child);

	T & value();
	const T & value() const;
	void setValue(T && val);

	size_t totalTreeSize() const;
	size_t childrensSize() const;

	bool isLeaf() const;
	void removeChild(const size_t & index);

	void insertChild(const int & index, Tree<T> &&child);
	void appendChild(Tree<T> && child);
	void prependChild(Tree<T> && child);
	Tree<T> & childAt(const size_t & index);
	const Tree<T> & childAt(const size_t & index) const;

	Tree<T> & operator[](const size_t & index);
	const Tree<T> & operator[](const size_t & index) const;

	template <typename TFunct>
	void foreach_child(const TFunct & funct);
};

template <typename T>
Tree<T>::Tree(T && value) : m_val(std::move(value)){}

template<typename T>
void Tree<T>::setChild(const size_t &index, Tree<T> &&child) {
	m_children[index] = std::move(child);
}

template<typename T>
T &Tree<T>::value() {
	return m_val;
}

template<typename T>
void Tree<T>::setValue(T &&val) {
	m_val = std::move(val);
}

template<typename T>
size_t Tree<T>::totalTreeSize() const {
	size_t size(1);
	for (Tree<T> const & child : m_children){
		size += child.totalTreeSize();
	}
	return size;
}

template<typename T>
const T &Tree<T>::value() const {
	return m_val;
}

template<typename T>
size_t Tree<T>::childrensSize() const {
	return m_children.size();
}

template<typename T>
bool Tree<T>::isLeaf() const {
	return m_children.empty();
}

template<typename T>
void Tree<T>::removeChild(const size_t &index) {
	CaDebug_suppose(index < m_children.size());
	m_children.erase(m_children.begin() + index);
}

template<typename T>
void Tree<T>::insertChild(const int &index, Tree<T> &&child) {
	CaDebug_suppose(index < m_children.size());
	m_children.emplace(m_children.begin() + index, std::move(child));
}

template<typename T>
void Tree<T>::appendChild(Tree<T> &&child) {
	m_children.emplace_back(std::move(child));
}

template<typename T>
void Tree<T>::prependChild(Tree<T> &&child) {
	m_children.emplace(m_children.begin(), std::move(child));
}

template<typename T>
Tree<T> & Tree<T>::childAt(const size_t &index) {
	return m_children.at(index);
}

template<typename T>
const Tree<T> &Tree<T>::childAt(const size_t &index) const {
	return m_children.at(index);
}

template<typename T>
Tree<T> &Tree<T>::operator[](const size_t &index) {
	return m_children.at(index);
}

template<typename T>
const Tree<T> &Tree<T>::operator[](const size_t &index) const {
	return m_children.at(index);
}

template<typename T>
template<typename TFunct>
void Tree<T>::foreach_child(const TFunct &funct) {
	std::for_each(m_children.begin(), m_children.end(), funct);
}






















