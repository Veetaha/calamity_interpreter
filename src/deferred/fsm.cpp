#include <fsm.h>

#include <error.h>
#include <token.h>
#include <ctype.h>

FSMRead_Float FSM_readFloat(FSMRead_Float currentState, char signal){
    if (currentState == FSM_FLOAT_START_MANTISSA && signal == '\0')
        return FSM_FLOAT_UNTERMINATED;
    if (signal == '\0') return FSM_FLOAT_SHUTDOWN;
    switch (currentState){
        case FSM_FLOAT_LAUNCH:
        case FSM_FLOAT_READ_INTEGRAL_PART:{
            if (isdigit(signal))
                return FSM_FLOAT_READ_INTEGRAL_PART;
            else if (signal == TOKEN_LITERAL_MANTISSA_DELIMITER)
                return FSM_FLOAT_START_MANTISSA;
            else
                return FSM_FLOAT_SHUTDOWN;
        }
        case FSM_FLOAT_START_MANTISSA:{
            if (!isdigit(signal)) return FSM_FLOAT_UNTERMINATED;
            return FSM_FLOAT_READ_MANTISSA;
        }
        case FSM_FLOAT_READ_MANTISSA:{
            if (isdigit(signal))
                return FSM_FLOAT_READ_MANTISSA;
            else return FSM_FLOAT_SHUTDOWN;
        }
        default: Debug_shutdown("FSM_readFloat was supposed to be already shutdown");
    }
}

FSMRead_String FSM_readString(FSMRead_String currentState, char signal){
    if (signal == '\0') return FSM_STRING_UNTERMINATED_STRING;
    switch (currentState) {
        case FSM_STRING_LAUNCH:
        case FSM_STRING_READ_ESCAPES:
        case FSM_STRING_READ_STRING:{
            if (Token_isChar(signal)){
                if (signal == TOKEN_LITERAL_ESPACES_START)
                    return FSM_STRING_START_ESCAPES;
                else if (signal == TOKEN_LITERAL_FINISH_STRING)
                    return FSM_STRING_SHUTDOWN;
                else
                    return FSM_STRING_READ_STRING;
            } else {
                return FSM_STRING_UNHANDLED_CHAR;
            }
        }
        case FSM_STRING_START_ESCAPES:{
            if (Token_isEscapeId(signal)){
                return FSM_STRING_READ_ESCAPES;
            } else {
                return FSM_STRING_INVALID_ESCAPES;
            }
        }
        default: Debug_shutdown("FSM_readString was supposed to be shutdown");
    }
}


FSMRead_Character FSM_readCharacter(FSMRead_Character currentState, char signal){
    if (signal == '\0') return FSM_CHAR_UNTERMINATED_CHAR;
    switch (currentState) {
        case FSM_CHAR_LAUNCH:{
            if (Token_isChar(signal)){
                if (signal == TOKEN_LITERAL_ESPACES_START) return FSM_CHAR_START_ESCAPES;
                    else return FSM_CHAR_READ_CHAR;
            } else {
                return FSM_CHAR_UNHANDLED_CHAR;
            }
        }
        case FSM_CHAR_START_ESCAPES:{
            if (Token_isEscapeId(signal)){
                return FSM_CHAR_READ_ESCAPES;
            } else {
                return FSM_CHAR_INVALID_ESCAPES;
            }
        }
        case FSM_CHAR_READ_ESCAPES:
        case FSM_CHAR_READ_CHAR:{
            if (signal == '\'')
                return FSM_CHAR_SHUTDOWN;
            else
                return FSM_CHAR_UNTERMINATED_CHAR;
        }
        default: Debug_shutdown("FSM_readChar was supposed to be shutdown");
    }
}




FSMRead_Words FSM_readWord(FSMRead_Words currentState, wchar_t signal, AnswerWChar isValid){
    if (signal == '\0') return FSM_WORDS_SHUTDOWN;
    switch (currentState){
        case FSM_WORDS_LAUNCH:
        case FSM_WORDS_WAIT:
                return isValid(signal) ? FSM_WORDS_START_WORD : FSM_WORDS_WAIT;
        case FSM_WORDS_START_WORD:
        case FSM_WORDS_READ_WORD:
                return isValid(signal) ? FSM_WORDS_READ_WORD : FSM_WORDS_WAIT;
        case FSM_WORDS_SHUTDOWN:
            Debug_shutdown("FSM was supposed to be sutdown");
            return FSM_WORDS_SHUTDOWN;
    }
    return FSM_WORDS_SHUTDOWN;
}