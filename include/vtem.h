#pragma once

#include <iterator>
#include <set>
#include <iostream>
#include <utility>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include "error.h"
#include "message_exception.h"
// Moved macroes to meta.h, including it for mainaining legacy code.
#include "meta.h"

#define raii(CLEANUP...)                                                                     \
    if constexpr (const auto & ____lamda__([&](){CLEANUP;}); true)                           \
    if constexpr (const ::Vtem::Raii<decltype(____lamda__)> ____raii__(____lamda__); true)

#define local_raii(CLEANUP...)                                                 \
    const auto & ____local_lamda__([&](){CLEANUP;});                           \
    const ::Vtem::Raii<decltype(____local_lamda__)> ____local_raii__(____local_lamda__);


// Stands for 'Vitaha templates'
namespace Vtem {
    constexpr const char * const SPACED_ID_REGEX = R"~~(^\s*(?:\w\s?)+$)~~";
    constexpr const char * const IPV4_ADDRESS_REGEX = R"~~(^(?:(?:\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(?:\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$)~~";

    template <typename TChar, char ... characters>
    inline constexpr const TChar strlit[] = { static_cast<TChar>(characters)... , static_cast<TChar>(0) };

    template<typename... T>
    struct are_integral : public std::__and_<std::is_integral<T>...> {
    };


    template<typename... TSuspects>
    inline constexpr const bool are_integral_v = are_integral<TSuspects...>::value;


    template<typename TSuspect>
    struct is_numeric : public std::integral_constant<bool,
            std::is_arithmetic_v<std::remove_reference_t<TSuspect>>
    > {
    };

    template<typename TSuspect>
    inline constexpr const bool is_numeric_v = is_numeric<TSuspect>::value;
    template<typename TSuspect>
    inline constexpr const bool is_class_or_union_v = std::is_class_v<TSuspect> || std::is_union_v<TSuspect>;


    template<typename T, std::size_t N>
    constexpr std::size_t size(const T (& arr)[N]) noexcept;


    template<typename TLeft, typename TRight>
    inline std::enable_if_t<
            (std::is_pointer_v<TLeft> && std::is_pointer_v<TRight>)
            || is_class_or_union_v<TLeft>
            || is_class_or_union_v<TRight>
            || (std::is_signed_v<TLeft> && std::is_signed_v<TRight>)
            || (std::is_unsigned_v<TLeft> && std::is_unsigned_v<TRight>),
            bool
    >
    isLess(const TLeft & left, const TRight & right) noexcept {
        return left < right;
    }

    template<typename TLeft, typename TRight>
    inline std::enable_if_t<
            std::is_unsigned_v<TLeft> && std::is_signed_v<TRight>,
            bool
    >
    isLess(const TLeft & left, const TRight & right) noexcept {
        return right > 0
               && left < static_cast<std::make_unsigned_t<TRight>>(right);
    }

    template<typename TLeft, typename TRight>
    inline std::enable_if_t<
            std::is_signed_v<TLeft> && std::is_unsigned_v<TRight>,
            bool
    >
    isLess(const TLeft & left, const TRight & right) noexcept {
        return left < 0
               || static_cast<std::make_unsigned_t<TLeft>>(left) < right;
    }


    template<typename TLeft, typename TRight>
    inline std::enable_if_t<
            (std::is_pointer_v<TLeft> && std::is_pointer_v<TRight>)
            || is_class_or_union_v<TLeft>
            || is_class_or_union_v<TRight>
            || (std::is_signed_v<TLeft> && std::is_signed_v<TRight>)
            || (std::is_unsigned_v<TLeft> && std::is_unsigned_v<TRight>),
            bool
    >
    isGreater(const TLeft & left, const TRight & right) noexcept {
        return left > right;
    }

    template<typename TLeft, typename TRight>
    inline std::enable_if_t<
            are_integral_v<TLeft, TRight> &&
            std::is_unsigned_v<TLeft>
            && std::is_signed_v<TRight>,
            bool
    >
    isGreater(const TLeft & left, const TRight & right) noexcept {
        return right < 0
               || left > static_cast<std::make_unsigned_t<TRight>>(right);
    }

    template<typename TLeft, typename TRight>
    std::enable_if_t<
            std::is_signed_v<TLeft> && std::is_unsigned_v<TRight>,
            bool
    >
    isGreater(const TLeft & left, const TRight & right) noexcept {
        return left > 0
               && static_cast<std::make_unsigned_t<TLeft>>(left) > right;
    }

    template<typename TLeft, typename TRight>
    bool isGeq(const TLeft & left, const TRight & right) noexcept {
        return !isLess(left, right);
    }

    template<typename TLeft, typename TRight>
    bool isLeq(const TLeft & left, const TRight & right) noexcept {
        return !isGreater(left, right);
    }


    template<typename TLeft, typename TRight>
    inline std::enable_if_t<
            std::is_floating_point_v<TLeft>
            || std::is_floating_point_v<TRight>,
            bool
    >
    isEqual(const TLeft & left, const TRight & right) noexcept {
        return std::abs(left - right) < std::max(
                std::numeric_limits<TLeft>::epsilon(),
                std::numeric_limits<TRight>::epsilon()
        );
    }


    template<typename TLeft, typename TRight>
    inline std::enable_if_t<
            (std::is_pointer_v<TLeft> && std::is_pointer_v<TRight>)
            || is_class_or_union_v<TLeft>
            || is_class_or_union_v<TRight>
            || (are_integral_v<TLeft, TRight> && (
			       (std::is_signed_v  <TLeft> && std::is_signed_v  <TRight>)
			    || (std::is_unsigned_v<TLeft> && std::is_unsigned_v<TRight>))
            ),
			bool
	>
	isEqual(const TLeft & left, const TRight & right) noexcept {
		return left == right;
	}

	template <typename TLeft, typename TRight>
	inline std::enable_if_t<
			std::is_unsigned_v<TLeft> && std::is_signed_v<TRight>,
			bool
	>
	isEqual(const TLeft & left, const TRight & right) noexcept {
		return right >= 0
			   && left == static_cast<std::make_unsigned_t<TRight>>(right);
	}

	template <typename TLeft, typename TRight>
	std::enable_if_t<
			std::is_signed_v<TLeft> && std::is_unsigned_v<TRight>,
			bool
	>
	isEqual(const TLeft & left, const TRight & right) noexcept {
		return left >= 0
			   && static_cast<std::make_unsigned_t<TLeft>>(left) == right;
	}

	template<typename TLeft, typename TRight>
	bool isNotEqual(const TLeft & left, const TRight & right) noexcept {
		return !isEqual(left, right);
	}

	template <typename TIter>
	void deletePtrs(TIter begin, const TIter & end);

	template <typename TContainer>
	void deletePtrs(TContainer & cont);

	template <typename TIter>
	void deleteOverlappingPtrs(TIter begin, const TIter & end);

	template <typename TContainer>
	void deleteOverlappingPtrs(TContainer & cont);

	template <typename A, typename B, typename C>
	bool inbound(const A &suspect, const B &left, const C &right);

	const char * toString(bool boolean);

	template <typename TContainer, typename TNumeric>
	bool hasIndex(const TContainer & cont, const TNumeric &index);

	template <typename TLimit, typename TValue>
	TValue mapToMaxbound(const TValue &value, const TLimit &max);

	template <typename TLimit, typename TValue>
	TValue mapToMinbound(const TValue &value, const TLimit &min);

	template <typename TLimits, typename TValue>
	TLimits map(const TValue &value, const TLimits &left, const TLimits &right);

	int doubleCompare(const double &left, const double &right, const double &epsilon = 1e-6);
	bool doubleEquals(const double &left, const double &right, const double &epsilon = 1e-6);

	template <typename TContainer, typename TChar>
	void print(const TContainer & cont, std::basic_ostream<TChar> & stream);


	template <typename T, std::size_t N>
	void print(const T (&arr)[N]);


	template <typename TContainer, typename TValue>
	auto find(const TContainer & container, const TValue & value);

	template <typename TContainer, typename TPredicate>
	auto tryFindReferenceIf(TContainer && container, TPredicate && predicate)
		-> decltype(*(container.begin())) &&;


	template <typename TContainer, typename TValue>
	bool contains(const TContainer & container, const TValue & value);

	template <typename TMinMaxEnum, typename TIntegral>
	inline bool enumContains(const TIntegral & suspect);

	template <typename TRange, typename TSuspect>
	std::enable_if_t<!std::is_same_v<TRange, TSuspect>, bool>
	isInRangeOf(const TSuspect & suspect);

	template <typename TRange>
	bool isInRangeOf(const TRange & suspect);

	template <typename TPtrContainer, typename TKey>
	auto tryFindPointer(const TPtrContainer & container,
						const TKey & key)
	-> typename std::remove_reference<decltype(*container.end())>::type;

	/* throws MessageException */
	template <typename TContainer, typename TKey>
	auto tryFindReference(const TContainer & container,
							const TKey & key)
	-> std::add_lvalue_reference_t<decltype(*container.begin())> &&;

	template <typename TNumeric>
	TNumeric & tryDecrement(TNumeric & number);

	template <typename TNumeric>
	TNumeric & tryIncrement(TNumeric & number);


	template <typename... TNumeric>
	double sum(TNumeric &&... values);

	template <typename... TNumeric>
	double average(TNumeric &&... values);

	/* beware might cause much runtime overhead */
	template <typename TContainer>
	auto at(TContainer && container, const size_t & index) -> decltype(*container.begin()) &;


	template <typename TContainer, typename TString, typename PushBacker>
	TString & appendContainerToString(TString & string, const TContainer & container, const PushBacker & appender);

	template <typename TContainer, typename TString>
	TString & appendContainerToString(TString & string, const TContainer & container);


	template <typename TNumeric, typename TString>
    std::enable_if_t<is_numeric_v<TNumeric>, TString &>
    appendNumberToString(TString & string, const TNumeric & number);

	template <typename TContainer, typename TString, typename TAppender>
	TString & prependToString(TString & string, const TContainer & container, const TAppender & appender);

	template <typename TNumeric, typename TString>
    auto prependToString(TString & string, const TNumeric & number)
    -> std::enable_if_t<is_numeric_v<TNumeric>, TString &>;

	template <typename TString, typename TChar>
    std::enable_if_t<std::is_same_v<TChar, typename TString::value_type>,
    TString &> prependToString(TString & string, const TChar * const & cString);

	template <typename TString>
	TString & prependToString(TString & string, const TString & prependString);

    template <typename TString, typename TChar, size_t N>
    TString & prependToString(TString &string, const TChar(&charArr)[N]);


	template <typename CharT, typename Traits>
	auto streamContentSize(std::basic_istream<CharT, Traits> & stream);


	template <typename TTemplate, typename... TSuspects>
	bool equalsOneOf(const TTemplate & templ, const TSuspects &... suspects);

	constexpr double nan() noexcept {
		return std::numeric_limits<double>::quiet_NaN();
	}

	constexpr double infinity() noexcept {
		return std::numeric_limits<double>::infinity();
	}

	template <typename TFunct>
	class Raii {
        const TFunct & m_onEscape;
	public:
	    explicit Raii(const TFunct & onBlockEscape) : m_onEscape(onBlockEscape) {}
	    ~Raii() { m_onEscape(); }
	};

	template <typename TChar>
	class IsWhitespace {
		std::locale m_globalLocale;
	public:
		inline bool operator()(const TChar & suspect) const {
			return std::isspace(suspect, m_globalLocale);
		}
	};

	template <typename TPredicate>
	class NegatedPredicate {
	    const TPredicate & m_predicate;
	public:
	    NegatedPredicate(const TPredicate & predicate) : m_predicate(predicate) {}
	    template <typename... TN>
	    inline bool operator()(TN &&... a) const {
            return !m_predicate(std::forward<TN>(a)...);
	    }

	};

	template <typename TChar>
	size_t cstrlen(const TChar * const & cString);



// -------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------


	template <typename T, std::size_t N>
	constexpr std::size_t size(const T (&arr)[N]) noexcept{
		return N;
	}



	template <typename TIter>
	void deletePtrs(TIter begin, const TIter & end){
		while (begin != end){
			delete *begin;
			++begin;
		}
	}

	template <typename TContainer>
	void deletePtrs(TContainer & cont){
		for (auto & ptr : cont){
			delete ptr;
		}
	}

	template <typename TIter>
	void deleteOverlappingPtrs(TIter begin, const TIter & end){
		if (begin == end) return;
		std::set<decltype(*begin)> set;
		while (begin != end){
			auto occurence = set.find(*begin);
			if (occurence == set.end()){
				delete *begin;
				set.insert(*begin);
			} // else already deleted
		}
	}

	template <typename TContainer>
	void deleteOverlappingPtrs(TContainer & cont){
		deleteOverlappingPtrs(cont.begin(), cont.end());
	}

	template <typename A, typename B, typename C>
	bool inbound(const A &suspect, const B &left, const C &right){
		return suspect >= left && suspect <= right;
	}

	template <typename TContainer, typename TNumeric>
	bool hasIndex(const TContainer & cont, const TNumeric &index){
		return isLess(index, cont.size()) && index >= 0;
	}


	template <typename TLimit, typename TValue>
	TValue mapToMaxbound(const TValue &value, const TLimit &max){
		return isLess(max, value) ? max : value;
	}

	template <typename TLimit, typename TValue>
	TValue mapToMinbound(const TValue &value, const TLimit &min){
		return isLess(value, min) ? min : value;
	}

	template <typename TLimits, typename TValue>
	TLimits map(const TValue &value, const TLimits &left, const TLimits &right){
		return isLess(right, value)
				? right
				: isLess(value, left)
					? left
					: value;
	}

	template <typename TContainer, typename TChar>
	void print(const TContainer & cont, std::basic_ostream<TChar> & stream){
		if (cont.cbegin() == cont.cend()){ return; }
		stream << static_cast<TChar>('[') << *cont.cbegin();
		for (auto it = cont.cbegin() + 1; it != cont.cend(); ++it) {
			stream << static_cast<TChar>(',')
				   << static_cast<TChar>(' ')
                   << *it;
		}
		stream << static_cast<TChar>(']');
	}

	template <typename T, std::size_t N>
	void print(const T (&arr)[N]){
		if (N == 0) { return; }
		std::cout << "[" << arr[0];
		const T * end = arr + N;
		for (const T * it = arr + 1; it != end; ++it) {
			std::cout << ", " << *it;
		}
		std::cout << ']';
	}

	template <typename TContainer, typename TValue>
	bool contains(const TContainer & container, const TValue & value){
		return std::find(container.begin(), container.end(), value) != container.end();
	}

	template <typename TMinMaxEnum, typename TIntegral>
	inline bool enumContains(const TIntegral & suspect){
		return inbound(suspect, TMinMaxEnum::Min, TMinMaxEnum::Max);
	}



	template <typename TRange, typename TSuspect, typename TCommonSignedness>
	bool isInRangeOfHelper(const TSuspect & suspect,
						 const TCommonSignedness &,
						 const TCommonSignedness &)
	{
		return std::numeric_limits<TRange>::lowest() <= suspect
			 && suspect <= std::numeric_limits<TRange>::max(); // both are signed or unsigned
	}

	template <typename TRange, typename TSuspect>
	bool isInRangeOfHelper(const TSuspect & suspect, const std::true_type &, const std::false_type &)
	{
		return suspect <= static_cast<std::make_unsigned_t<TRange>>(
						  std::numeric_limits<TRange>::max() // suspect is unsigned, range is signed
		);
	}

	template <typename TRange, typename TSuspect>
	bool isInRangeOfHelper(const TSuspect & suspect, const std::false_type &, const std::true_type &)
	{
		return 0 <= suspect // range is unsigned, suspect is signed
				&& static_cast<std::make_unsigned_t<TSuspect>>(suspect) <= std::numeric_limits<TRange>::max();
	}

	template <typename TRange, typename TSuspect, typename TFloatOrInt, typename TFloatOrInt2>
	bool isInRangeOf_integralCheckHelper(const TSuspect & suspect,
										 const TFloatOrInt  &,
										 const TFloatOrInt2 &){
		return std::numeric_limits<TRange>::lowest() <= suspect // one of the values is floating point
				&& suspect <= std::numeric_limits<TRange>::max();
	}


	template <typename TRange, typename TSuspect>
	bool isInRangeOf_integralCheckHelper(const TSuspect & suspect,
										 const std::true_type &,
										 const std::true_type &){
		return isInRangeOfHelper<TRange>(
			suspect,
			std::is_signed<TRange>(),      // both are integral types
			std::is_signed<TSuspect>()
		);
	}

	template <typename TRange, typename TSuspect>
	std::enable_if_t<!std::is_same_v<TRange, TSuspect>, bool>
	isInRangeOf(const TSuspect & suspect){
		return isInRangeOf_integralCheckHelper<TRange>(
			suspect,
			typename std::is_integral<TRange>::type(),
			typename std::is_integral<TSuspect>::type()
		);
	}

	template <typename TRange>
	bool isInRangeOf(const TRange &){
		return true;
	}

	template <typename TPtrContainer, typename TKey>
	auto tryFindPointer(const TPtrContainer & container,
						const TKey & key)
	-> typename std::remove_reference<decltype(*container.end())>::type
	{
		auto resultIterator { container.find(key) };
		if (resultIterator == container.end()){
			Debug_logError("failed to find pointer for according key");
			return nullptr;
		} else {
			return *resultIterator;
		}
	}


	template <typename TContainer, typename TKey>
	auto tryFindReference(const TContainer & container,
							const TKey & key)
	-> std::add_lvalue_reference_t<decltype(*container.begin())> &&
	{
		auto resultIterator { container.find(key) };
		if (resultIterator == container.end()){
			throw ::MessageException("failed to find reference for according key");
		} else {
			return *resultIterator;
		}
	}

	template <typename TContainer, typename TValue>
	auto find(const TContainer & container, const TValue & value){
		return std::find(container.begin(), container.end(), value);
	}

	template <typename TNumeric>
	TNumeric & tryDecrement(TNumeric & number){
		if (number < std::numeric_limits<TNumeric>::lowest() + 1){
			number = std::numeric_limits<TNumeric>::lowest();
			return number;
		} else {
			--number;
			return number;
		}

	}

	template <typename TNumeric>
	TNumeric & tryIncrement(TNumeric & number){
		if (number > std::numeric_limits<TNumeric>::max() - 1){
			number = std::numeric_limits<TNumeric>::max();
			return number;
		} else {
			++number;
			return number;
		}

	}

	template <typename TLastNumber>
	double sum_helper(TLastNumber && lastNumber){
		return static_cast<double>(lastNumber);
	}


	template <typename TCurrent, typename... TNumeric>
	double sum_helper(TCurrent && current, TNumeric &&... values){
		return static_cast<double>(current) + sum_helper(values...);
	}


	template <typename... TNumeric>
	double sum(TNumeric &&... values){
		return sum_helper(values...);
	}

	template <typename... TNumeric>
	double average(TNumeric &&... values){
		return sum(values...) / sizeof...(values);
	}

	template <typename TContainer>
	auto at(TContainer && container, const size_t & index) -> decltype(*container.begin()) &{
		return container.begin() + index;
	}


	template <typename TContainer, typename TPredicate>
	auto tryFindReferenceIf(TContainer && container, TPredicate && predicate)
		-> decltype(*(container.begin())) &&
	{
		auto resultIterator (
			std::find_if(container.begin(), container.end(), predicate)
		);
		if (resultIterator == container.end()){
			throw ::MessageException("predicate failed to find reference");
		} else {
			return *resultIterator;
		}
	}


	template <typename TContainer, typename TString, typename PushBacker>
	TString & appendContainerToString(
            TString & string,
            const TContainer & container,
            const PushBacker & appender
    ){
	    typedef std::remove_reference_t<decltype(string[0])> char_type;
		auto begin(container.begin());
		auto end(container.end());
		if (begin == end){
            return (string += static_cast<char_type>('['))
                           += static_cast<char_type>(']');
		} else {
			string += static_cast<char_type>('[');
			appender(string, *begin);
			++begin;
			while (begin != end){
                (string += static_cast<char_type>(','))
                        += static_cast<char_type>(' ');
				appender(string, *begin);
				++begin;
			}
			return string += static_cast<char_type>(']');
		}
	}


	template<typename TContainer, typename TString>
	TString & appendContainerToString(TString & string, const TContainer & container) {
		return appendContainerToString(string, container,
			[](TString & string, const auto & item){
				string += item;
			}
		);
	}


	template <typename TNumeric, typename TString>
    std::enable_if_t<is_numeric_v<TNumeric>,
    TString &> appendNumberToString(TString & string, const TNumeric & number){
		return string += ::boost::lexical_cast<TString>(number);
	}

	template<typename TContainer, typename TString, typename TAppender>
	TString & prependToString(TString &string, const TContainer &container, const TAppender &appender) {
		TString temporary;
		string.insert(0, appendContainerToString(temporary, container, appender));
		return string;
	}

	template<typename TNumeric, typename TString>
    auto prependToString(TString & string, const TNumeric & number)
    -> std::enable_if_t<is_numeric_v<TNumeric>, TString &>{
		string.insert(0, boost::lexical_cast<TString>(number));
		return string;
	}

    template<typename TString, typename TChar, size_t N>
    TString & prependToString(TString &string, const TChar(&charArr)[N]) {
        if (string.capacity() < string.size() + N){
            string.reserve(string.size() + N);
        }
        string.insert(0, charArr, N);
        return string;
    }


    template <typename TString, typename TChar>
    std::enable_if_t<std::is_same_v<TChar, typename TString::value_type>,
    TString & >prependToString(TString & string, const TChar * const & cString){
		string.insert(0, cString);
		return string;
	}

	template<typename TString>
	TString & prependToString(TString &string, const TString &prependString) {
	    if (string.capacity() < string.size() + prependString.size()) {
            string.reserve(string.size() + prependString.size());
        }
		string.insert(0, prependString);
	    return string;
	}

	template<typename CharT, typename Traits>
	auto streamContentSize(std::basic_istream<CharT, Traits> & stream) {
		if (!stream.good()){
			Debug_logError(
				"basic_istream<CharT, Traits> in error "
				"state was forwarded to query its content size"
			);
			return static_cast<typename std::basic_istream<CharT, Traits>::pos_type>(0);
		}
		auto prevPos(stream.tellg());
		auto size(stream.seekg(0, std::ios_base::end).tellg());
		stream.seekg(prevPos);
		return size;
	}


    template<typename TTemplate, typename TSuspect>
    bool equalsOneOf_helper(const TTemplate & templ, const TSuspect & suspect) {
        return isEqual(templ, suspect);
    }

	template <typename TTemplate, typename TSuspect1, typename... TSuspectN>
	bool equalsOneOf_helper(
	        const TTemplate & templ,
            const TSuspect1 & suspect1,
            const TSuspectN &... suspects){
	    return isEqual(templ, suspect1) || equalsOneOf_helper(templ, suspects...);
	}

    template<typename TTemplate, typename... TSuspects>
    bool equalsOneOf(const TTemplate & templ, const TSuspects & ... suspects) {
        return equalsOneOf_helper(templ, suspects...);
    }

    template <typename TChar>
    size_t cstrlen(const TChar * const & cString) {
        return std::char_traits<TChar>::length(cString);
    }



}

