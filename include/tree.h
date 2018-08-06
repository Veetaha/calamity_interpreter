#pragma once

#include <vector>
#include <utility>
#include <algorithm>
#include "vtem.h"
#include "meta.h"
#include "error.h"

namespace Vtem {

    template <typename T>
    class Tree {
    public:
        typedef std::vector<Tree<T> > children_vector;
    private:

        T m_val;
        children_vector m_children;
    public:
        DECL_DEFAULT_MOVE_ONLY(Tree)


        meta_inline explicit Tree(T && initialValue = T(), children_vector && children = children_vector());

        meta_inline void setChild(const size_t &index, Tree<T> && child);
        meta_inline void setChildren(children_vector && childrenRval);
        meta_inline void reserveChildren(const size_t & size);
        meta_inline void reserveMoreChildren(const size_t & size);
        meta_inline Tree * childPtr(const size_t & index) &;

        meta_inline Tree && rval() & {
            return static_cast<Tree &&>(*this);
        }

        /**
         * @brief Tree::value and Tree::operator*() (indirection) access the value
         * stored in the tree node.
         */
        meta_inline T & value() &;
        meta_inline const T & value() const &;
        meta_inline T value() && = delete;

        meta_inline T & operator*() &;
        meta_inline const T & operator*() const &;
        meta_inline T operator*() && = delete;
        meta_inline T * operator->() &;
        meta_inline const T * operator->() const &;
        meta_inline Tree & operator+=(Tree && child) &;
        meta_inline Tree & operator<<(Tree && child) &;
        meta_inline Tree & operator+=(children_vector && children) &;
        meta_inline Tree & operator<<(children_vector && children) &;

        template <typename TString>
        TString & appendToString(TString & string);

        /**
         * @brief   Same as *tree = val;
         */
        meta_inline void setValue(T && val);

        meta_inline size_t totalTreeSize() const;
        meta_inline size_t childrenSize() const;

        meta_inline bool isLeaf() const;
        meta_inline void removeChild(const size_t & index);

        meta_inline void insertChild(const int & index, Tree && child);
        meta_inline void appendChild(Tree && child);
        meta_inline void prependChild(Tree && child);

        /**
         * @brief   Returns reference to a child node at the specified index.
         * Debug statement is provided to check wether index < childrenSize()
         */
        meta_inline Tree & childAt(const size_t & index) &;
        meta_inline const Tree & childAt(const size_t & index) const &;
        meta_inline Tree & lastChild() &;

        /**
         * @brief Identical to Tree::childAt(index)
         */
        meta_inline Tree & operator[](const size_t & index) &;
        meta_inline const Tree & operator[](const size_t & index) const &;

        /**
         * calls a function void (*)(T &) on each childNode
         */
        template <typename TFunct>
        meta_inline void foreach_child(const TFunct & funct);
    };

    template <typename T>
    Tree<T>::Tree(T && value, children_vector && children)
    : m_val(std::move(value)), m_children(std::move(children)) {}


    template <typename T>
    Tree<T> & Tree<T>::operator+=(children_vector && children) & {
        reserveMoreChildren(children.size());
        for (Tree<T> & child : children) {
            m_children.emplace_back(std::move(child));
        }
        return *this;
    }

    template <typename T>
    Tree<T> & Tree<T>::operator<<(children_vector && children) & {
        return *this += std::move(children);
    }

    template <typename T>
    Tree<T> & Tree<T>::lastChild() & {
        Debug_suppose(!isLeaf());
        return m_children.back();
    }

    template <typename T>
    void Tree<T>::reserveChildren(const size_t & size){
        if (m_children.capacity() < size) {
            m_children.reserve(size);
        }
    }

    template <typename T>
    void Tree<T>::reserveMoreChildren(const size_t & size){
        m_children.reserve(size + m_children.size());
    }


    template<typename T>
    void Tree<T>::setChild(const size_t &index, Tree<T> &&child) {
        m_children[index] = std::move(child);
    }

    template <typename T>
    void Tree<T>::setChildren(children_vector && childrenRval){
        m_children = std::move(childrenRval);
    }

    template<class T> const T &Tree<T>::value() const &         { return m_val;  }
    template<class T> T &Tree<T>::value() &                     { return m_val;  }
    template<class T> const T &Tree<T>::operator*() const &     { return m_val;  }
    template<class T> T &Tree<T>::operator*() &                 { return m_val;  }
    template<class T> T * Tree<T>::operator->() & { return &m_val; }
    template<class T> const T * Tree<T>::operator->() const &   { return &m_val; }

    template<typename T>
    Tree<T> & Tree<T>::operator+=(Tree && child) & {
        m_children.emplace_back(std::move(child));
        return *this;
    }

    template<typename T>
    Tree<T> & Tree<T>::operator<<(Tree && child) & {
        m_children.emplace_back(std::move(child));
        return *this;
    }


    template<typename T>
    void Tree<T>::setValue(T &&val) {
        m_val = std::move(val);
    }

    template<typename T>
    size_t Tree<T>::totalTreeSize() const {
        size_t size(1);
        for (const Tree<T> & child : m_children){
            size += child.totalTreeSize();
        }
        return size;
    }


    template<typename T>
    size_t Tree<T>::childrenSize() const { return m_children.size(); }

    template<typename T>
    bool Tree<T>::isLeaf() const {
        return m_children.empty();
    }

    template<typename T>
    void Tree<T>::removeChild(const size_t &index) {
        Debug_suppose(index < m_children.size());
        m_children.erase(m_children.begin() + index);
    }

    template<typename T>
    void Tree<T>::insertChild(const int &index, Tree<T> &&child) {
        Debug_suppose(index <= m_children.size());
        m_children.emplace(m_children.begin() + index, std::move(child));
    }

    template<typename T>
    void Tree<T>::appendChild(Tree<T> && child) {
        m_children.emplace_back(std::move(child));
    }

    template<typename T>
    void Tree<T>::prependChild(Tree<T> && child) {
        m_children.emplace(m_children.begin(), std::move(child));
    }

    template<typename T>
    Tree<T> & Tree<T>::childAt(const size_t &index) & {
        Debug_suppose(index < m_children.size());
        return m_children.at(index);
    }

    template<typename T>
    const Tree<T> &Tree<T>::childAt(const size_t &index) const & {
        Debug_suppose(index < m_children.size());
        return m_children.at(index);
    }

    template<typename T>
    Tree<T> & Tree<T>::operator[](const size_t &index) & {
        Debug_suppose(index < m_children.size());
        return m_children[index];
    }

    template<typename T>
    const Tree<T> &Tree<T>::operator[](const size_t &index) const & {
        Debug_suppose(index < m_children.size());
        return m_children[index];
    }

    template<typename T>
    template<typename TFunct>
    void Tree<T>::foreach_child(const TFunct &funct) {
        std::for_each(m_children.begin(), m_children.end(), funct);
    }

    template <typename T>
    Tree<T> * Tree<T>::childPtr(const size_t & index) &{
        Debug_suppose(index < m_children.size());
        return &m_children[index];
    }


    namespace TreePrivate {
        template<typename TChar>
        class Indent { };

        #define TreeImpl_specializeIndent(TYPE, PREFIX...)               \
        template<>                                                       \
        struct Indent<TYPE> {                                            \
            static constexpr const TYPE * const VLINE = PREFIX##"┃ ";    \
            static constexpr const TYPE * const INBRANCH = PREFIX##"┣━"; \
            static constexpr const TYPE * const OUTBRANCH = PREFIX##"┗━";\
        };
        TreeImpl_specializeIndent(char)
        TreeImpl_specializeIndent(wchar_t, L)
        TreeImpl_specializeIndent(char16_t, u)
        TreeImpl_specializeIndent(char32_t, U)

        template <typename TVal, typename TString>
        void appendToString(
            TString & dest,
            const Tree<TVal> & self,
            TString & indent,
            const bool & isLast,
            const bool & isRoot
        );
    }


    template <typename T>
    template <typename TString>
    TString & Tree<T>::appendToString(TString & string){
        TString indent;
        TreePrivate::appendToString(string, *this, indent, true, true);
        return string;
    }


    namespace TreePrivate {


        template <typename TVal, typename TString>
        void appendToString(
            TString & dest,
            const Tree<TVal> & self,
            TString & indent,
            const bool & isLast,
            const bool & isRoot
        ){
            typedef typename TString::value_type TChar;
            typedef Indent<TChar> Indentation;
            dest += indent;
            TString localIndent(indent);
            if (isLast) {
                if (!isRoot) {
                    dest += Indentation::OUTBRANCH;
                    localIndent += TChar(' ');
                    localIndent += TChar(' ');
                }
                //
            } else {
                dest        += Indentation::INBRANCH;
                localIndent += Indentation::VLINE;
            }
            dest << *self;
            dest += TChar('\n');
            for (size_t i = 0; i < self.childrenSize(); ++i) {
                appendToString(
                    dest,
                    self[i],
                    localIndent,
                    i == (self.childrenSize() - 1),
                    false
                );
            }
        }

    }
}













































