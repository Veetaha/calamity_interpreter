#pragma once

#include <iostream>
#include "defs.h"

namespace Cala {
	class Boolean {
		bool m_bool;

	public:
		DECL_DEFAULT_COPY_AND_MOVE(Boolean)

		Boolean() = default;
		inline Boolean(const bool &b);

		static inline Boolean cast(const Null &);
		static inline Boolean cast(const Undefined &);
		static inline Boolean cast(const Number & number);
		static inline Boolean cast(const Integer & integer);
		static inline Boolean cast(const String & string);
		static inline Boolean cast(const List & list);
		static inline Boolean cast(const Object & obj);
		static inline Boolean cast(const Function & func);


		inline explicit operator bool() const;
		/**
		 * @return Bool primitive value wrapped by this class.
		 */
		inline bool value() const;
		/**
		 * @return Bool primitive value wrapped by this class.
		 */
		inline bool operator*() const;

		inline Boolean &operator=(const bool &boolean);
		inline friend ostream &operator<<(ostream &stream, const Boolean &self);

		inline void print(ostream &stream = conout);

		template <typename TBoolean>
		bool operator==(const TBoolean &other) const;

		template <typename TBoolean>
		bool operator!=(const TBoolean &other) const;

		inline bool operator!() const;

		// returns new toggled value
		inline bool toggle();
	};
}


#include "var_boolean_inline_impl.h"