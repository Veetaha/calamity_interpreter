#pragma once

#include <iostream>
#include "std_ext.h"
#include "var_string.h"

namespace Calamity {

	class Boolean {
		bool m_bool;

	public:
		DECL_DEFAULT_COPY_AND_MOVE(Boolean)

		Boolean() = default;
		Boolean(const bool &b);

		explicit operator bool() const;
		bool value() const;

		Boolean &operator=(const bool &boolean);
		friend std::wostream &operator<<(std::wostream &stream, const Boolean &self);

		void print(std::wostream &stream = std::wcout) { stream << *this; }

		const String & toString() const;
        const cachar_t * toCaString() const;
		const char *toCString() const;
		const wchar_t *toWCString() const;
		const char16_t *toU16CString() const;


		bool operator==(const Boolean &other) const;
		bool operator!=(const Boolean &other) const;

		// returns new toggled value
		bool toggle();
	};
}