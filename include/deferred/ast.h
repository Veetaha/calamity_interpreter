#pragma once
#include "tree.h"
#include "var.h"
#include "point2d.h"
#include "var_string.h"


namespace Calamity {


	class AstNode {
	public:
		enum class Type {
			Unknown,

			And,
			Eq,
			Geq,
			Gt,
			Leq,
			Lt,
			Neq,
			Not,
			Or,

			Assign,
			Division,
			Minus,
			Multiplication,
			Plus,
			Power,
			Remainder,

			Literal,
			Identifier,

			Arglist,
			Block,
			Case,
			While,
			Declaration,
			Calamity,

			List_literal,
			Blank_statement,
			Prim_arglist,

			Define,
			Return
		};
	private:
		Type m_type;
		Var m_value;
		BasicString m_name;
		Point2D<> m_location;
	public:

		Type type() const;

		Var & value();
		const Var & value() const;
		Var && rvalue() const;

		const BasicString & name() const;

		const Point2D<> & location() const;

		void setType(const Type & type);

		void setValue(Var && rvalue);

		void setName(BasicString && name);

		void setLocation(Point2D<> && location);


		explicit AstNode(
			const Type & type = Type::Unknown,
			Point2D<> loc = Point2D(),
			BasicString && name = "noname",
			Var && rvalue = Var()
		);
		BasicString toString();
		BasicString & appendToString(BasicString & str);

		std::ostream & operator<<(std::ostream & stream, const AstNode & self);
		const char * toCString(Type type);

	private:
		class Typaname {
			static constexpr const char * const Unknown =        "Unknown";
			static constexpr const char * const And =            "and";
			static constexpr const char * const Eq =             "==";
			static constexpr const char * const Geq =            ">=";
			static constexpr const char * const Gt =             ">";
			static constexpr const char * const Leq =            "<=";
			static constexpr const char * const Less =           "<";
			static constexpr const char * const Neq =            "!=";
			static constexpr const char * const Not =            "!";
			static constexpr const char * const Or =             "or";
			static constexpr const char * const Assign =         "=";
			static constexpr const char * const Divide =         "/";
			static constexpr const char * const Minus =          "-";
			static constexpr const char * const Multiply =       "*";
			static constexpr const char * const Plus =           "+";
			static constexpr const char * const Power =          "^";
			static constexpr const char * const Remainder =      "%";
			static constexpr const char * const Literal =        "LL";
			static constexpr const char * const Identifier =     "ID";
			static constexpr const char * const Arglist =        "Arglist";
			static constexpr const char * const Block =          "Block";
			static constexpr const char * const Case =           "case";
			static constexpr const char * const While =          "while";
			static constexpr const char * const Declaration =    "Declare";
			static constexpr const char * const Calamity =       "Calamity";
			static constexpr const char * const List_literal =   "compound_literal";
			static constexpr const char * const Blank_statement ="Blank statement";
			static constexpr const char * const Prim_arglist =   "Prim arglist";
			static constexpr const char * const Define =         "define";
			static constexpr const char * const Return =         "return";


			static const char *AST_TYPE_NAMES[] = {
					Unknown,
					And,
					Eq,
					Geq,
					Gt,
					Leq,
					Less,
					Neq,
					Not,
					Or,
					Assign,
					Divide,
					Minus,
					Multiply,
					Plus,
					Power,
					Remainder,
					Literal,
					Identifier,
					Arglist,
					Block,
					Case,
					While,
					Declaration,
					Calamity,
					List_literal,
					Blank_statement,
					Prim_arglist,
					Define,
					Return
			};
		};
	};

	typedef Tree<AstNode> Ast;
}