#include <limits>
#include "var_number.h"
#include "var_boolean.h"
#include "vtem.h"

namespace Cala {

    const Number Number::nan      = Vtem::nan();
    const Number Number::infinity = Vtem::infinity();
    const Number Number::max      = std::numeric_limits<float_t>::max();
    const Number Number::min      = std::numeric_limits<float_t>::min();


    template <typename TPredIsSpace = IsWhitespace>
    static signed char getNumberSign(
        const String & self,
        const TPredIsSpace & isspace = TPredIsSpace()
    ){
        signed char sign(1);
        for (const cachar_t & character : self){
            if (character == static_cast<cachar_t>('-')){
                sign *= -1;
            } else if (!isspace(character)) {
                return sign;
            }
        }
        return sign;
    }


    Number Number::parseBeginTrimmed(const String & string){
        return parseBeginTrimmed(Substring(string.cbegin(), string.cend()));
    }

    Number Number::parseBeginTrimmed(const Substring &){
        Debug_noimpl();


    }



    Number Number::parse(const String & string) noexcept {
        try {
            return std::stod(string.string());
        } catch (const std::invalid_argument &) {
            return NaN;
        } catch (const std::out_of_range &) {
            return getNumberSign(string) * Infinity;
        }
    }


    Number Number::cast(const String & string) noexcept {
        try {
            size_t lastDigitIndex;
            double number(std::stod(string.string(), &lastDigitIndex));
            
            if (std::find_if(string.begin() + lastDigitIndex, string.end(), IsWhitespace()) 
                    == string.end()){
                return number;
            }                                      // nan
        } catch (const std::invalid_argument &) {} // nan
          catch (const std::out_of_range &) {
            // TODO: check if there is no trailing grabage in the string
            // set the proper sign for infinity
            return getNumberSign(string) * Infinity;
        }
        return NaN;
    }


    static Number parse(Substring && substr) noexcept {
        substr.trimBegin();
        Debug_noimpl();
    }
}