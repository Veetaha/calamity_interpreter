#pragma once
#include <cassert>
#include <clocale>
#include <exception>
#include <iostream>
#include "meta.h"

namespace Debug {

#ifndef DEBUG_OSTREAM
    #define DEBUG_OSTREAM ::std::cout
#endif
#ifndef DEBUG_CHAR_TYPE
    #define DEBUG_CHAR_TYPE char
#endif
    using char_type = DEBUG_CHAR_TYPE;


#define imply(presupposition, conclusion) (!(presupposition) || (conclusion))

#ifndef NO_DEBUG
	#define __DEBUG_CHAR_PREFIX Macro_charPrefix(DEBUG_CHAR_TYPE)
	#define __Debug_str(STRING) Macro_concatenate(__DEBUG_CHAR_PREFIX, STRING)

    #define Debug_exists(expression) assert((expression) && "An attempt to dereference NULL pointer was made");
	#define Debug_inbound(suspect, leftBound, rightBound) assert((leftBound) <= (suspect) && (suspect) <= (rightBound) && "Value out of bounds exception");
	#define Debug_maxbound(suspect, rightBound) assert((suspect) <= (rightBound) && "Value out of bounds exception");
	#define Debug_minbound(suspect, leftBound) assert((suspect) >= (leftBound) && "Value out of bounds exception");
	#define Debug_suppose_EXT(expression, errorMessage, ...) assert((expression) && (errorMessage))
    #define Debug_suppose(expression, errorMessage...) (Debug_suppose_EXT((expression), ##errorMessage, "Supposition failure"))
    #define Debug_shutdown(reason) assert(0 && (reason));
	#define Debug_maxindex(suspect, maximum) assert(std_ext::inbound((suspect), 0, (maximum)) && "Index out of bounds exception");
    #define Debug_supposeIf(CONDITION, SUPPOSITION, MESSAGE...)\
    	Debug_suppose(!(CONDITION) || SUPPOSITION, ##MESSAGE)


	#define Debug_noimpl() Debug_shutdown(__Debug_str("program workflow has reached unimplemented line of code"))
    #define Debug_unreachable() Debug_shutdown(__Debug_str("program workflow has reached an unreachable line of code"))

	#ifndef NO_DEBUG_LOG
		#define Debug_log(MESSAGE...)                      \
			(DEBUG_OSTREAM << __Debug_str("Debug_log at ") \
							<< __func__                    \
							<< __Debug_str(" -> ")         \
							<< MESSAGE                     \
							<< __Debug_str('\n'))
		#define Debug_logError(MESSAGE...)                           \
			(DEBUG_OSTREAM << __Debug_str("************* error at ") \
						   << __func__                               \
						   << __Debug_str(" -> ")                    \
						   << MESSAGE                                \
						   << __Debug_str('\n'))
		#define Debug_logIf(CONDITION, MESSAGE) ({                              \
			if ((CONDITION)){                                                   \
				(DEBUG_OSTREAM << __Debug_str("Debug_logIf -> ")                \
					           << MESSAGE                                       \
					           << __Debug_str(" | triggered by condition ")     \
					           << Macro_stringify(CONDITION));                  \
			}                                                                   \
		})
    #else
		#define Debug_log(MESSAGE...)
		#define Debug_logError(MESSAGE...)
	#endif
    #ifndef NO_DEBUG_TRACE

        #ifndef DEBUG_TRACE_CALL_TOKEN
            #define DEBUG_TRACE_CALL_TOKEN __Debug_str('*')
        #endif
        #ifndef DEBUG_TRACE_RETURN_TOKEN
            #define DEBUG_TRACE_RETURN_TOKEN __Debug_str('-')
        #endif
        class FunctionStackframe{
            static inline int sm_stackLevel = 0;
            const char * m_funcName;
            static void printHLine(int size, const char_type &token){
                while (size--){ DEBUG_OSTREAM << token; }
            }
        public:
            template <typename TMessage>
            explicit FunctionStackframe(const char * const & funcName, TMessage && message)
                : m_funcName(funcName) {
                printHLine(++sm_stackLevel, DEBUG_TRACE_CALL_TOKEN);
                DEBUG_OSTREAM << __Debug_str("[") << m_funcName << __Debug_str("] ")
                    << message;
                DEBUG_OSTREAM << __Debug_str('\n');
            }
            explicit  FunctionStackframe(const char * const & funcName)
                : FunctionStackframe(funcName, __Debug_str("")) {}
            ~FunctionStackframe() {
                printHLine(--sm_stackLevel, DEBUG_TRACE_RETURN_TOKEN);
                DEBUG_OSTREAM << __Debug_str("<")  << m_funcName
                              << __Debug_str("<<") << __Debug_str("\n");
            }
            static const int & stackLevel(){
                return sm_stackLevel;
            }
        };

        #define TRACE_CALL(MESSAGE...) ::Debug::FunctionStackframe Macro_anonVar()(__func__, ##MESSAGE);
    #else
    #endif
#else
    #define Debug_exists(expression)
	#define Debug_inbound(suspect, leftBound, rightBound)
	#define Debug_maxbound(suspect, rightBound)
	#define Debug_minbound(suspect, leftBound)
	#define Debug_suppose_EXT(expression, errorMessage, ...)
    #define Debug_suppose(expression, errorMessage...)
    #define Debug_shutdown(reason) assert(0 && (reason));
	#define Debug_maxindex(suspect, maximum)
    #define Debug_supposeIf(CONDITION, SUPPOSITION, MESSAGE...)
	#define Debug_noimpl()
    #define Debug_unreachable()
	#define Debug_log(MESSAGE...)
	#define Debug_logError(MESSAGE...)
	#define Debug_logIf(CONDITION, MESSAGE)
    #define TRACE_CALL()
    #define TRACE_CALL_LINE()
#endif
}
