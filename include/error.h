#pragma once
#include <cassert>
#include <clocale>
#include <exception>
#include <iostream>

namespace Debug {



#define imply(presupposition, conclusion) (!(presupposition) || (conclusion))

#ifndef NO_DEBUG
    #define Debug_exists(expression)  assert((expression) && "An attempt to dereference NULL pointer was made");
    #define Debug_UTF8() assert(strcmp(setlocale(0, NULL), "C") && "An attempt to print wide character without UTF-8 format was made")
	#define Debug_inbound(suspect, leftBound, rightBound) assert((leftBound) <= (suspect) && (suspect) <= (rightBound) && "Value out of bounds exception");
	#define Debug_maxbound(suspect, rightBound) assert((suspect) <= (rightBound) && "Value out of bounds exception");
	#define Debug_minbound(suspect, leftBound) assert((suspect) >= (leftBound) && "Value out of bounds exception");
	#define Debug_suppose_EXT(expression, errorMessage, ...) assert((expression) && (errorMessage))
    #define Debug_suppose(expression, errorMessage...) (Debug_suppose_EXT((expression), ##errorMessage, "Supposition failure"))
    #define Debug_shutdown(reason) assert(0 && (reason))
	#define Debug_maxindex(suspect, maximum) assert(std_ext::inbound((suspect), 0, (maximum)) && "Index out of bounds exception")
    #ifndef NO_DEBUG_TIPS
        #define Debug_tip(expression, tipMessage) assert((expression) && (tipMessage))
    #else
        #define Debug_tip(expression, tipMessage)
    #endif

	#define Debug_noimpl() Debug_shutdown("program workflow has reached unimplemented line of code")
    #define Debug_unreachable() Debug_shutdown("program workflow has reached an unreachable line of code")

	#ifndef NO_DEBUG_LOG
		#define Debug_log(MESSAGE...) \
			(std::cout << "Debug_log at " << __func__ << " -> " << MESSAGE << '\n')
		#define Debug_logError(MESSAGE...) \
			(std::cout << "************* error at " << __func__ << " -> " << MESSAGE << '\n')
		#define Debug_logIf(CONDITION, MESSAGE) ({                                                        \
			if ((CONDITION)){                                                                             \
				(std::cout << "Debug_logIf -> " << MESSAGE << " | triggered by condition " << #CONDITION);\
			}                                                                                             \
		})
#else
		#define Debug_log(MESSAGE...)
		#define Debug_logError(MESSAGE...)
	#endif



    #ifndef NO_DEBUG_TRACE

    unsigned short _____getLevel(); // return current function stack height
    void _____stackPushFunc(const char *func);

    void _____stackPopFunc(const char **func);



        #define TRACE_CALL()                        \
                _____stackPushFunc(__func__);          \
                const char * _____escapedFunction __attribute__((cleanup(_____stackPopFunc))) = __func__

        #define TRACE_CALL_LINE() TRACE_CALL(); putchar('\n');
    #else
        #define TRACE_CALL()
        #define TRACE_CALL_LINE()
    #endif

#else
    #define Debug_exists(expression)
    #define Debug_UTF8()
    #define Debug_inbound(leftBound, suspect, rightBound)
    #define Debug_maxbound(suspect, rightBound)
    #define Debug_minbound(suspect, leftBound)
    #define Debug_suppose(expression, errorMessage)
    #define Debug_shutdown(reason)
	#define QLOG(message)
	#define tip(expression, tipMessage)

    #define TRACE_CALL()
    #define TRACE_CALL_LINE()
#endif
}
