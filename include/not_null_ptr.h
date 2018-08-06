#pragma once
#include <cassert>
#include "vtem.h"

namespace Vtem {

    template <typename T>
    class NotNullPtr;
    #if !defined(NO_DEBUG) && !defined(NO_NULLPTR_CHECKS)
        #define CP_check(MEMORY) assert((MEMORY) && "SIGSEGV: *nullptr")

        template <typename TPointer>
        using NotNull = NotNullPtr<std::remove_pointer_t<TPointer> >;

        #ifdef INCREMENT_NULLPTR_CHECKS
            #define CP_incrementCheck(MEMORY) CP_check((MEMORY))
        #else
            #define CP_incrementCheck(MEMORY)
        #endif
    #else
        template <typename TPointer>
        using NotNull = TPointer;

        #define CP_incrementCheck(MEMORY)
        #define CP_check(MEMORY)
    #endif





    /**
     * @brief   Debug special pointer class to get a diagnostic message
     *  when dereferencing a nullptr.
     *
     * @tparam T type of entity this pointer is refering to.
     *
     * NotNullPtr never owns a pointer and never deletes it.
     */
    template <typename T>
    class NotNullPtr {
        T * m_ptr;
    public:
        typedef T * wrapped_type;

        DECL_DEFAULT_COPY_AND_MOVE(NotNullPtr)

        meta_inline T * const & value() const & {
            return m_ptr;
        }

        meta_inline T * & value() & {
            return m_ptr;
        }

        meta_inline T * value() && {
            return m_ptr;
        }

        NotNullPtr() : m_ptr(nullptr) {}
        NotNullPtr(T * const & ptr) : m_ptr(ptr) {}

        meta_inline bool isNull() const {
            return !m_ptr;
        }

        meta_inline bool isNotNull() const {
            return m_ptr;
        }

        meta_inline operator T * &() & {
            return m_ptr;
        }

        meta_inline operator T *() && {
            return m_ptr;
        }

        meta_inline explicit operator bool(){
            return m_ptr;
        }

        meta_inline T * const & operator->() const & {
            CP_check(m_ptr);
            return m_ptr;
        }

        meta_inline T * & operator->() & {
            CP_check(m_ptr);
            return m_ptr;
        }

        meta_inline T * operator->() && {
            CP_check(m_ptr);
            return m_ptr;
        }

        meta_inline T & operator*() {
            CP_check(m_ptr);
            return *m_ptr;
        }



        meta_inline NotNullPtr & operator++() & {
            CP_incrementCheck(m_ptr);
            return ++m_ptr, *this;
        }

        meta_inline const NotNullPtr operator++(int) & {
            CP_incrementCheck(m_ptr);
            return m_ptr++;
        }

        meta_inline NotNullPtr & operator--() & {
            CP_incrementCheck(m_ptr);
            return --m_ptr, *this;
        }

        meta_inline const NotNullPtr operator--(int) & {
            CP_incrementCheck(m_ptr);
            return m_ptr--;
        }

    Meta_wrapper_def_relationalOperator(NotNullPtr, m_ptr, ==)
    Meta_wrapper_def_relationalOperator(NotNullPtr, m_ptr, !=)
    Meta_wrapper_def_relationalOperator(NotNullPtr, m_ptr, >)
    Meta_wrapper_def_relationalOperator(NotNullPtr, m_ptr, <)
    Meta_wrapper_def_relationalOperator(NotNullPtr, m_ptr, >=)
    Meta_wrapper_def_relationalOperator(NotNullPtr, m_ptr, <=)

    Meta_wrapper_def_arithmeticAndAssignmentOperators(NotNullPtr, m_ptr, +)
    Meta_wrapper_def_arithmeticAndAssignmentOperators(NotNullPtr, m_ptr, -)


    };
}



