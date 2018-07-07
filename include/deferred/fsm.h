#pragma once
#include <wide_character.h>

typedef enum __FSMRead_Float{
    FSM_FLOAT_LAUNCH,
    FSM_FLOAT_SHUTDOWN,

    FSM_FLOAT_READ_INTEGRAL_PART,
    FSM_FLOAT_START_MANTISSA,
    FSM_FLOAT_READ_MANTISSA,

    FSM_FLOAT_UNTERMINATED
} FSMRead_Float;
FSMRead_Float FSM_readFloat(FSMRead_Float currentState, char signal);


// ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛
// ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛

typedef enum __FSMRead_String{
    FSM_STRING_LAUNCH,
    FSM_STRING_SHUTDOWN,

    FSM_STRING_READ_STRING,
    FSM_STRING_START_ESCAPES,
    FSM_STRING_READ_ESCAPES,

    
    FSM_STRING_UNHANDLED_CHAR,
    FSM_STRING_UNTERMINATED_STRING,
    FSM_STRING_INVALID_ESCAPES
} FSMRead_String;
FSMRead_String FSM_readString(FSMRead_String currentState, char signal);


// ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛
// ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛


typedef enum __FSMRead_Character{
    FSM_CHAR_LAUNCH,
    FSM_CHAR_SHUTDOWN,

    FSM_CHAR_READ_CHAR,
    FSM_CHAR_START_ESCAPES,
    FSM_CHAR_READ_ESCAPES,

    FSM_CHAR_UNHANDLED_CHAR,
    FSM_CHAR_UNTERMINATED_CHAR,
    FSM_CHAR_INVALID_ESCAPES
} FSMRead_Character;
FSMRead_Character FSM_readCharacter(FSMRead_Character currentState, char signal);


// ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛
// ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛


typedef enum __FSMRead_Words{
    FSM_WORDS_LAUNCH,

    FSM_WORDS_START_WORD,
    FSM_WORDS_READ_WORD,
    FSM_WORDS_WAIT,

    FSM_WORDS_SHUTDOWN
} FSMRead_Words;
FSMRead_Words FSM_readWord(FSMRead_Words currentState, wchar_t signal, AnswerWChar isValid);