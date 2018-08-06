#pragma once

#ifndef __cplusplus
	#define nullptr ((void *)0)
	#define _auto __extension__ __auto_type
    #define typeat(ARGS...) __typeof(ARGS)
    #define sizeat(arr) ({               \
        typedef typeat(arr) ArrType;     \
        sizeof(ArrType)/sizeof((arr)[0]);\
    })
    #define slitlen(literal) (sizeat(literal) - 1)

    #define FALL_THROUGH __attribute((fallthrough));
#else
    #include "stddef.h"
    #define _auto auto
    #define typeat(ARGS...) decltype(ARGS)
    namespace Meta {
        template<typename T, size_t N>
        constexpr size_t sizeat(const T (& arr)[N]) noexcept {
            return N;
        }
        template<typename TChar, size_t N>
        constexpr size_t slitlen(const TChar (& literalCharacterArray)[N]) noexcept {
            return N - 1;
        }
    }
    #define FALL_THROUGH [[fallthrough]]
#endif

#define Macro_concatenate_EXP(LEFT, RIGHT) LEFT##RIGHT
#define Macro_concatenate(LEFT, RIGHT) Macro_concatenate_EXP(LEFT, RIGHT)

#define Macro_stringify_EXP(ARGS...) #ARGS
#define Macro_stringify(ARGS...) Macro_stringify_EXP(ARGS)

#define Macro_overloadByPostfix(MACRONAME, POSTFIX, ARGS...)\
	Macro_concatenate(MACRONAME, POSTFIX)(##ARGS)

#define Macro_charPrefix_char32_t() U
#define Macro_charPrefix_char16_t() u
#define Macro_charPrefix_wchar_t()  L
#define Macro_charPrefix_char()
#define Macro_charPrefix(CHAR_TYPE) Macro_overloadByPostfix(Macro_charPrefix_, CHAR_TYPE)


#define meta_inline inline __attribute((always_inline))

#define AUTO_DESTR(destructor) __extension__ __attribute((cleanup(destructor)))

#define return_autoptr(AUTOMATIC_VARIABLE) ({ \
	_auto ___temp = (AUTOMATIC_VARIABLE);     \
	AUTOMATIC_VARIABLE = nullptr;			  \
	___temp;								  \
})

#define DECL_UNCOPIABLE(Type)				\
	Type(const Type &) = delete;			\
	Type & operator=(const Type &) = delete;

#define DECL_UNMOVABLE(Type)			\
	Type(Type &&) = delete;				\
	Type & operator=(Type &&) = delete;

#define DECL_NO_COPY_AND_MOVE(Type) DECL_UNCOPIABLE(Type) DECL_UNMOVABLE(Type)


#define DECL_DEFAULT_COPYING(Type)				\
	Type(const Type &) = default;				\
	Type & operator=(const Type &) = default;	\

#define DECL_DEFAULT_MOVING(Type)			        \
	Type(Type &&) noexcept = default;				\
	Type & operator=(Type &&) noexcept = default;	\

#define DECL_DEFAULT_COPY_AND_MOVE(Type) DECL_DEFAULT_MOVING(Type) DECL_DEFAULT_COPYING(Type)

#define DECL_DEFAULT_MOVE_ONLY(Type) DECL_DEFAULT_MOVING(Type) DECL_UNCOPIABLE(Type)

#define DEPRECATED __attribute((deprecated))
#define DEPRECATED_WHY(why) __attribute((deprecated(why)))

#define begin_exprst ({
#define end_exprst   })


#define Macro_invoke(ARGS...) ARGS

#define Macro_argument_20(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, ...) _20
#define Macro_argument_19(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, ...) _19
#define Macro_argument_18(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, ...) _18
#define Macro_argument_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, ...) _17
#define Macro_argument_16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define Macro_argument_15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define Macro_argument_14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define Macro_argument_13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define Macro_argument_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define Macro_argument_11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define Macro_argument_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define Macro_argument_9( _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define Macro_argument_8( _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define Macro_argument_7( _1, _2, _3, _4, _5, _6, _7, ...) _7
#define Macro_argument_6( _1, _2, _3, _4, _5, _6, ...) _6
#define Macro_argument_5( _1, _2, _3, _4, _5, ...) _5
#define Macro_argument_4( _1, _2, _3, _4, ...) _4
#define Macro_argument_3( _1, _2, _3, ...) _3
#define Macro_argument_2( _1, _2, ...) _2
#define Macro_argument_1( _1, ...) _1

#define Macro_argumentsAmount(ARGS...) \
Macro_argument_20("dummy", ##ARGS, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define Macro_hasArgument_0(...) absent
#define Macro_hasArgument_1(...) present
#define Macro_hasArgument_2(...) present
#define Macro_hasArgument_3(...) present
#define Macro_hasArgument_4(...) present
#define Macro_hasArgument_5(...) present
#define Macro_hasArgument_6(...) present
#define Macro_hasArgument_7(...) present
#define Macro_hasArgument_8(...) present
#define Macro_hasArgument_9(...) present
#define Macro_hasArgument_10(...) present
#define Macro_hasArgument_11(...) present
#define Macro_hasArgument_12(...) present
#define Macro_hasArgument_13(...) present
#define Macro_hasArgument_14(...) present
#define Macro_hasArgument_15(...) present
#define Macro_hasArgument_16(...) present
#define Macro_hasArgument_17(...) present
#define Macro_hasArgument_18(...) present
#define Macro_hasArgument_19(...) present
#define Macro_hasArgument_20(...) present



#define Macro_overload(PREFIX, ARGS...)\
	Macro_concatenate(PREFIX##_, Macro_argumentsAmount(ARGS))(ARGS)

#define Macro_hasArgument(ARGUMENT...)\
		Macro_overload(Macro_hasArgument, ##ARGUMENT)

#ifdef __COUNTER__
    #define Macro_anonVar() Macro_concatenate(__anonymousVar_, __COUNTER__)
#else
    #define Macro_anonVar() Macro_concatenate(__anonymousVar_, __LINE__)
#endif




#define Macro_nameOverloadedByPresence(PREFIX, ARGS...)  \
	Macro_concatenate(PREFIX##_, Macro_hasArgument(ARGS))


#define Macro_foreachArg_2( CALLBACKFN, ARG) CALLBACKFN(ARG)
#define Macro_foreachArg_3( CALLBACKFN, ARG, ARGS)    CALLBACKFN(ARG) Macro_foreachArg_2 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_4( CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_3 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_5( CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_4 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_6( CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_5 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_7( CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_6 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_8( CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_7 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_9( CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_8 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_10(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_9 (CALLBACKFN, ##ARGS)
#define Macro_foreachArg_11(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_10(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_12(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_11(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_13(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_12(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_14(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_13(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_15(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_14(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_16(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_15(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_17(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_16(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_18(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_17(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_19(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_18(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_20(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_19(CALLBACKFN, ##ARGS)
#define Macro_foreachArg_21(CALLBACKFN, ARG, ARGS...) CALLBACKFN(ARG) Macro_foreachArg_20(CALLBACKFN, ##ARGS)

#define Macro_foreachArg(CALLBACKFN, ARGS...)\
	Macro_overload(Macro_foreachArg, CALLBACKFN, ##ARGS)


#define Meta_wrapper_def_prePostOperators(WRAPPER, FIELD, OPERATOR)\
meta_inline const WRAPPER operator OPERATOR (int) & {                \
    return FIELD OPERATOR;                                           \
}                                                                    \
meta_inline WRAPPER & operator OPERATOR () & {                       \
    return (OPERATOR FIELD), *this;                                  \
}



#define Meta_wrapper_def_relationalOperator(WRAPPER, FIELD, OPERATOR)                        \
meta_inline bool operator OPERATOR(const WRAPPER & other) const {                            \
	return FIELD OPERATOR other . FIELD;                                                     \
}                                                                                            \
friend meta_inline bool operator OPERATOR(const wrapped_type & self, const WRAPPER & other) {\
	return self OPERATOR other . FIELD;                                                      \
}


#define Meta_wrapper_def_assignmentOperator(WRAPPER, FIELD, OPERATOR__APPLY...)\
Macro_overload(Meta_wrapper_def_assignmentOperator, WRAPPER, FIELD, ##OPERATOR__APPLY)

/**
 * @brief Defines trivial assignment operator.
 */
#define Meta_wrapper_def_assignmentOperator_2(WRAPPER, FIELD) \
Meta_wrapper_def_assignmentOperator_3(WRAPPER, FIELD, )

/**
 * @brief Defines assignment operators for wrapper type.
 * @OPERATOR Operator sign, that must not be a compound operator.
 * It is a primitive part of compound assignment.
 * E.g. in order to define operator&=, forward only & as an OPERATOR.
 * @APPLY Macro or function name that is called when applying operator
 * as APPLY(lhs, OPERATOR, rhs).
 * Lhs and rhs are of the wrapped_type type.
 *
 *
 * * WRAPPER class must have an inner [typedef type_it_wraps wrapped_type;]
 * or [using wrapped_type = type_it_wraps;]
 *
 * Beware to invoke this macro inside a WRAPPER class definition.
 */
#define Meta_wrapper_def_assignmentOperator_3(WRAPPER, FIELD, OPERATOR)                \
meta_inline WRAPPER & operator OPERATOR##=(const WRAPPER & lval) & {                   \
	FIELD OPERATOR##= lval . FIELD;                                                    \
	return *this;                                                                      \
}                                                                                      \
meta_inline WRAPPER & operator OPERATOR##=(WRAPPER && rval) & {                        \
    FIELD OPERATOR##= static_cast<wrapped_type &&>(rval . FIELD);                      \
    return *this;                                                                      \
}                                                                                      \
meta_inline WRAPPER & operator OPERATOR##=(const wrapped_type & lval) & {              \
	FIELD OPERATOR##= lval;                                                            \
	return *this;                                                                      \
}                                                                                      \
meta_inline WRAPPER & operator OPERATOR##=(wrapped_type && rval) & {                   \
	FIELD OPERATOR##= static_cast<wrapped_type &&>(rval);                              \
	return *this;                                                                      \
}

/**
 * @brief Defines assignment opperators for wrapper type.
 * @OPERATOR Operator sign, that must not be a compound operator.
 * It is a primitive part of compound assignment.
 * E.g. in order to define operator&=, forward only & as an OPERATOR.
 * @APPLY Macro or function name that is called when applying operator
 * as APPLY(lhs, OPERATOR, rhs).
 * Lhs and rhs are of the wrapped_type type.
 *
 * * WRAPPER class must have an inner [typedef type_it_wraps wrapped_type;]
 * or [using wrapped_type = type_it_wraps;]
 *
 * Beware to invoke this macro inside a WRAPPER class definition.
 */
#define Meta_wrapper_def_assignmentOperator_4(WRAPPER, FIELD, OPERATOR, APPLY)         \
meta_inline WRAPPER & operator OPERATOR##=(const WRAPPER & lval) & {                   \
	FIELD = APPLY(FIELD, OPERATOR, lval . FIELD);                                      \
	return *this;                                                                      \
}                                                                                      \
meta_inline WRAPPER & operator OPERATOR##=(WRAPPER && rval) & {                        \
    FIELD = APPLY(FIELD, OPERATOR, static_cast<wrapped_type &&>(rval . FIELD));        \
    return *this;                                                                      \
}                                                                                      \
meta_inline WRAPPER & operator OPERATOR##=(const wrapped_type & lval) & {              \
	FIELD = APPLY(FIELD, OPERATOR, lval);                                              \
	return *this;                                                                      \
}                                                                                      \
meta_inline WRAPPER & operator OPERATOR##=(wrapped_type && rval) & {                   \
	FIELD = APPLY(FIELD, OPERATOR, static_cast<wrapped_type &&>(rval));                \
	return *this;                                                                      \
}


#define Meta_wrapper_def_arithmeticBinaryOperator(WRAPPER, FIELD, OPERATOR, APPLY...)\
    Macro_overload(Meta_wrapper_def_arithmeticBinaryOperator, WRAPPER, FIELD, OPERATOR, ##APPLY)
/**
 * @brief Defines all the permutation of WRAPPER and WRAPPER::wrapped_type binary
 * arithmetic operators.
 *
 * WRAPPER class must have an inner [typedef type_it_wraps wrapped_type;]
 * or [using wrapped_type = type_it_wraps;]
 *
 * Beware to invoke this macro inside a WRAPPER class definition.
 */
#define Meta_wrapper_def_arithmeticBinaryOperator_3(WRAPPER, FIELD, OPERATOR)  \
meta_inline WRAPPER && operator OPERATOR(const WRAPPER & other) && {           \
	FIELD OPERATOR ##= other . FIELD;                                          \
	return static_cast<WRAPPER &&>(*this);                                     \
}     												                           \
meta_inline WRAPPER && operator OPERATOR(WRAPPER && other) && {                \
	FIELD OPERATOR ##= static_cast<wrapped_type &&>(other . FIELD);            \
	return static_cast<WRAPPER &&>(*this);                                     \
}     												                           \
meta_inline WRAPPER && operator OPERATOR(WRAPPER && other) const &  {          \
	other . FIELD = FIELD OPERATOR static_cast<wrapped_type &&>(other . FIELD);\
	return static_cast<WRAPPER &&>(other);                                     \
}     												                           \
meta_inline WRAPPER operator OPERATOR(const WRAPPER & other) const &  {        \
	return WRAPPER(FIELD OPERATOR other . FIELD);                              \
}     												                           \
meta_inline WRAPPER && operator OPERATOR(const wrapped_type & other) && {      \
	FIELD OPERATOR##= other;                                                   \
	return static_cast<WRAPPER &&>(*this);                                     \
}     												                           \
meta_inline WRAPPER && operator OPERATOR(wrapped_type && other) && {           \
	FIELD OPERATOR ##= static_cast<wrapped_type &&>(other);                    \
	return static_cast<WRAPPER &&>(*this);                                     \
}     												                           \
meta_inline WRAPPER operator OPERATOR(wrapped_type && other) const &  {        \
	return WRAPPER(FIELD OPERATOR static_cast<wrapped_type &&>(other));        \
}     												                           \
meta_inline WRAPPER operator OPERATOR(const wrapped_type & other) const &  {   \
	return WRAPPER(FIELD OPERATOR other);                                      \
}     												                           \
friend meta_inline WRAPPER operator OPERATOR (                                 \
	const wrapped_type & left, const WRAPPER & right                           \
){                                                                             \
	return WRAPPER(left OPERATOR right . FIELD);                               \
}                                                                              \
friend meta_inline WRAPPER operator OPERATOR (                                 \
	wrapped_type && left, const WRAPPER & right	                               \
){                                                                             \
	return WRAPPER(static_cast<wrapped_type &&>(left) OPERATOR right . FIELD); \
}                                                                              \
friend meta_inline WRAPPER && operator OPERATOR (                              \
	const wrapped_type & left, WRAPPER && right	                               \
){                                                                             \
	right . FIELD = left OPERATOR static_cast<wrapped_type &&>(right . FIELD); \
	return static_cast<WRAPPER &&>(right);                                     \
}                                                                              \
friend meta_inline WRAPPER && operator OPERATOR (                              \
	wrapped_type && left, WRAPPER && right	                                   \
){                                                                             \
	right . FIELD = static_cast<wrapped_type &&>(left)                         \
			OPERATOR                                                           \
					static_cast<wrapped_type &&>(right . FIELD);               \
	return static_cast<WRAPPER &&>(right);                                     \
}

/**
 * @brief Same as macro Meta_wrapper_def_arithmeticBinaryOperator,
 * but applies operator OPERATOR via calling ot function or macro
 * APPLY(lhs, OPERATOR, rhs).
 * Lhs and rhs are of wrapped_type type.
 */
#define Meta_wrapper_def_arithmeticBinaryOperator_4(WRAPPER, FIELD, OPERATOR, APPLY)       \
meta_inline WRAPPER && operator OPERATOR(const WRAPPER & other) && {                       \
	FIELD = APPLY(                                                                         \
	    static_cast<wrapped_type &&>(FIELD),                                               \
	    OPERATOR,                                                                          \
	    other . FIELD                                                                      \
	);                                                                                     \
	return static_cast<WRAPPER &&>(*this);                                                 \
}     												                                       \
meta_inline WRAPPER && operator OPERATOR(WRAPPER && other) && {                            \
	FIELD = APPLY(                                                                         \
	    static_cast<wrapped_type &&>(FIELD),                                               \
	    OPERATOR,                                                                          \
	    static_cast<wrapped_type &&>(other . FIELD)                                        \
	);                                                                                     \
	return static_cast<WRAPPER &&>(*this);                                                 \
}     												                                       \
meta_inline WRAPPER && operator OPERATOR(WRAPPER && other) const &  {                      \
	other . FIELD = APPLY(                                                                 \
	    FIELD,                                                                             \
	    OPERATOR,                                                                          \
	    static_cast<wrapped_type &&>(other . FIELD)                                        \
	);                                                                                     \
	return static_cast<WRAPPER &&>(other);                                                 \
}     												                                       \
meta_inline WRAPPER operator OPERATOR(const WRAPPER & other) const &  {                    \
	return WRAPPER(                                                                        \
	    APPLY(                                                                             \
	        FIELD,                                                                         \
	        OPERATOR,                                                                      \
	        other . FIELD                                                                  \
	    )                                                                                  \
    );                                                                                     \
}     												                                       \
meta_inline WRAPPER && operator OPERATOR(const wrapped_type & other) && {                  \
	FIELD = APPLY(                                                                         \
	    static_cast<wrapped_type &&>(FIELD),                                               \
	    OPERATOR,                                                                          \
	    other                                                                              \
    );                                                                                     \
	return static_cast<WRAPPER &&>(*this);                                                 \
}     												                                       \
meta_inline WRAPPER && operator OPERATOR(wrapped_type && other) && {                       \
	FIELD = APPLY(                                                                         \
	    static_cast<wrapped_type &&>(FIELD),                                               \
	    OPERATOR,                                                                          \
	    static_cast<wrapped_type &&>(other)                                                \
    );                                                                                     \
	return static_cast<WRAPPER &&>(*this);                                                 \
}     												                                       \
meta_inline WRAPPER operator OPERATOR(wrapped_type && other) const &  {                    \
	return WRAPPER(                                                                        \
	    APPLY(                                                                             \
	        FIELD,                                                                         \
	        OPERATOR,                                                                      \
	        static_cast<wrapped_type &&>(other)                                            \
        )                                                                                  \
	);                                                                                     \
}     												                                       \
meta_inline WRAPPER operator OPERATOR(const wrapped_type & other) const &  {               \
	return WRAPPER(APPLY(FIELD, OPERATOR, other));                                         \
}     												                                       \
friend meta_inline WRAPPER operator OPERATOR (                                             \
	const wrapped_type & left, const WRAPPER & right                                       \
){                                                                                         \
	return WRAPPER(APPLY(left, OPERATOR, right . FIELD));                                  \
}                                                                                          \
friend meta_inline WRAPPER operator OPERATOR (                                             \
	wrapped_type && left, const WRAPPER & right	                                           \
){                                                                                         \
	return WRAPPER(                                                                        \
	    APPLY(                                                                             \
	        static_cast<wrapped_type &&>(left),                                            \
	        OPERATOR,                                                                      \
	        right . FIELD                                                                  \
        )                                                                                  \
	);                                                                                     \
}                                                                                          \
friend meta_inline WRAPPER && operator OPERATOR (                                          \
	const wrapped_type & left, WRAPPER && right	                                           \
){                                                                                         \
	right . FIELD = APPLY(                                                                 \
	    left,                                                                              \
	    OPERATOR,                                                                          \
	    static_cast<wrapped_type &&>(right . FIELD)                                        \
    );                                                                                     \
	return static_cast<WRAPPER &&>(right);                                                 \
}                                                                                          \
friend meta_inline WRAPPER && operator OPERATOR (                                          \
	wrapped_type && left, WRAPPER && right	                                               \
){                                                                                         \
	right . FIELD = APPLY(                                                                 \
	    static_cast<wrapped_type &&>(left),                                                \
		OPERATOR,                                                                          \
		static_cast<wrapped_type &&>(right . FIELD)                                        \
    );                                                                                     \
	return static_cast<WRAPPER &&>(right);                                                 \
}

#define Meta_wrapper_def_arithmeticAndAssignmentOperators(WRAPPER, FIELD, OPERATOR, APPLY...)\
    Meta_wrapper_def_arithmeticBinaryOperator(WRAPPER, FIELD, OPERATOR, ##APPLY)\
    Meta_wrapper_def_assignmentOperator(WRAPPER, FIELD, OPERATOR, ##APPLY)

