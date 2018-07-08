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

#define AUTO_DESTR(destructor) __extension__ __attribute((cleanup(destructor)))

#define return_autoptr(AUTOMATIC_VARIABLE) ({ \
	_auto ___temp = (AUTOMATIC_VARIABLE);     \
	AUTOMATIC_VARIABLE = nullptr;			  \
	___temp;								  \
})


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

#define Macro_concatenate_EXP(LEFT, RIGHT) LEFT##RIGHT
#define Macro_concatenate(LEFT, RIGHT) Macro_concatenate_EXP(LEFT, RIGHT)

#define Macro_stringify_EXP(ARGS...) #ARGS
#define Macro_stringify(ARGS...) Macro_stringify_EXP(ARGS)

#define Macro_overload(PREFIX, ARGS...)\
	Macro_concatenate(PREFIX##_, Macro_argumentsAmount(ARGS))(ARGS)

#define Macro_hasArgument(ARGUMENT...)\
		Macro_overload(Macro_hasArgument, ##ARGUMENT)


#define Macro_nameOverloadedByPresence(PREFIX, ARGS...)\
	Macro_concatenate(PREFIX##_, Macro_hasArgument(ARGS))

