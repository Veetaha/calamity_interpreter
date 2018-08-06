#pragma once
#include "defs.h"
#include "tree.h"
#include "var.h"
#include "point2d.h"
#include "var_string.h"
#include "token.h"


namespace Cala {

    /* TODO: {
     *      add: {
     *          { type: Enum }
     *          { type: Import }
     *          { type: Export }
     *          { type: Yield }
     *          { type: Yield* }
     *          { type: Ellipsis, descr: "spread operator, or function rest parameter" }
     *      }
     * }
     */
	class AstNode {
	public:
		enum class Type {
            Unknown = 0, // {}
            Calamity,    // children: [Statement[0], ..., Statement[N]]
            TernaryCondition,
            /* ^^^^^^ {
             *  children: [
             *      Expression[Ternary::Condition],
             *      Expression[Ternary::TrueExpr],
             *      Expression[Ternary::FalseExpr]
             *  ]
             *}
             */

            LogAnd,
            Assign,
            PlusAssign,
            MinusAssign,
            MultiplyAssign,
            DivideAssign,
            RemainderAssign,
            PowerAssign,
            BitAndAssign,
            BitOrAssign,
            BitXorAssign,
            BitLShiftAssign,
            Bit3RShiftAssign,
            Bit2RShiftAssign,
            BitAnd,
            BitOr,
            BitXor,
            BitLeftShift,
            Bit3RightShift,
            Bit2RightShift,
            Coma,
            Division,
            Equality,
            GreaterOrEqual,
            GreaterThan,
            LessOrEqual,
            LessThan,
            Minus,// may be unary
            Multiplication,
            LogOr,
            Plus, // may be unary
            Power,
            Remainder,

            Identity,
            NotEquality,
            NotIdentity,
            In,
            Instanceof,
            /* ^^^^^^ {
             *  children: [
             *      Expression[BinaryOperator::Left],
             *      Expression[BinaryOperator::Right]
             *  ]
             *}
             */
            BitNot,
            LogNot,
            Void,
            Await,
            Typeof,
            PreIncrement,
            PreDecrement,
            PostIncrement,
            PostDecrement,
            /* ^^^^^
             *  { children: [Expression] }
             */
            New,
            /*{
             *  children: [Expression[0], ..., Expression[N]],
             *  name: <constructor_id>
             *}
             */
            This,     // {}
            Break,    // {}
            Continue, // {}

            Literal,    // { value: <literal_value> }
            Identifier, // { name: "<id>"}

            /* Paramlist, // removed */
            BlockSt, // { children: [ Statement[0], ..., Statement[N]] }

            While,
            DoWhile,
            /*{
             *  children: [
             *      Expression[While::Condition],
             *      Statement [While::Body]
             *  ]
             *}
             */
            ForVanilla,
            /*{
             *  children: [
             *      Expression[ForVanilla::Condition],
             *      Statement [ForVanilla::Body],
             *      Statement [ForVanilla::InitSt],
             *      (Statement[ForVanilla::EndSt]),
             *  ]
             *}
             */
            ForLetIn,
            ForVarIn,
            ForConstIn,
            ForLetOf,
            ForVarOf,
            ForConstOf,
            /*{
             *  children: [
             *      Expression[ForInOf::Target],
             *      Statement [ForInOf::Body]
             *  ],
             *  name: "<var_id>"
             *}
             */
            Let,
            Const,
            Var, // { children: [Vardecl[0], ..., Vardecl[N]] }
            Vardecl, // { children: [(Expression)], name: "<variable_id>" }
                     //             init~~~^
            ListLiteral, // { children: [Expression[0], ..., Expression[N] }
            BlankSt,

            Return,
            Throw,  // { children: [Expression] }
            Try,
            /*{
             *  children: [
             *      BlockSt   [Try::TryBlock],
             *      BlockSt   [Try::CatchBlock]
             *  ],
             *  name: <catch_var_id>
             *}
             */

            Switch,
            /*{
             *  children: [
             *      Expression   [Switch::Target],
             *      StatementPack[Switch::StatementPack],
             *      Cases        [Switch::Cases]
             *  ],
             *  value<Number>: <default_statement_index>
             *}
             */
            Cases, // { children: [Case[0], ..., Case[N]] }
            Case,  // { children: [Expression], value<Number>: <statement_index> }
            Delete,   // { children: [(Expression)], name: Indetifier|Expression}
                      //              object~~^     propname~~^
            Funcdecl,
            AsyncFuncdecl,
            GeneratorFuncdecl,
            AsyncGeneratorFuncdecl,
            /*{
             *  children: [
             *      Paramlist[Funcdecl::Paramlist],
             *      BlockSt  [Funcdecl::Body]
             *  ],
             *  name: "<func_id>"
             *}
             */
            Param, // { children: [Expression], name: "<arg_id>" }      // Note: same as Vardecl
                   //      default_value~~^
            IfSt,
            /*{
             *  children: [
             *      Expression[If::Condition],
             *      Statement [If::TrueBlock],
             *      (Statement[If::ElseBlock])
             *  ]
             *}
             */
            Dot,
            /*{
             *  children: [
             *      Expression[Dot::Target],
             *      Identifier[Dot::Propname]
             *  ]
             *}
             */
            Funclit,
            AsyncFunclit,
            GeneratorFunclit,
            AsyncGeneratorFunclit,
            /*{
             *  children: [
             *      BlockSt  [Funclit::Body],
             *      Paramlist[Funclit::Paramlist]
             *  ]
             *  name: "funclit_id"|""
             *}
             */
            Arrowfunc,
            /*{
             *  children: [
             *      Expression|BlockSt[Arrowfunc::Body]
             *      Paramlist         [Arrowfunc::Paramlist]
             *  ]
             *}
             */
            Paramlist,       // { children: [Param[0], ..., Param[N], (RestParam)] }
            CompMemberAccess,
            /*{
             *  children: [
             *      Expression[CMA::Target],
             *      Expression[CMA::Propname]
             *  ]
             *}
             */
            RestParam,
            StatementPack,   // { children: [Statement[0], ..., Statement[1]] }
            FnCall,
            /*{
             *  children: [
             *      Expression[FnCall::Function],
             *      ExprList  [FnCall::Args]
             *  ]
             *}
             */
            ExprList,     // { children: [Expression[0], ..., Expression[N]] }
            ObjLiteral,// { children: [ObjectProp[0], ..., ObjectProp[N]] }
            ObjProp
            /*{
             *  children: [
             *      Expression[ObjProp::Key],
             *      Expression[ObjProp::Value]
             *  ]
             *}
             */
		};

	private:
		Type m_type;
		Var  m_value;
		Substring m_name;
	public:

		meta_inline Type type() const           { return m_type;  }
		meta_inline Var & value() &             { return m_value; }
		meta_inline const Var & value() const & { return m_value; }
		meta_inline Var && rvalue() &  { return static_cast<Var &&>(m_value); }
		meta_inline Var && rvalue() && = delete;
		meta_inline const Substring & name() const &     { return m_name;    }
        meta_inline Substring & name()  &                { return m_name;    }
		meta_inline void setType(const Type & type)      { m_type  = type;   }
		meta_inline void setValue(Var && rvalue)         { m_value = rvalue; }
		meta_inline void setName(const Substring & name) { m_name  = name;   }
		meta_inline bool isValid() const { return m_type != Type::Unknown; }
		meta_inline bool is(const Type & type) const { return m_type == type; }


		explicit AstNode(
			const Type & type = Type::Unknown,
            const Substring & name = Substring(),
			Var && rvalue = Var()
		) : m_type(type), m_value(std::move(rvalue)), m_name(name) {}

        explicit AstNode(Var && rvalue)
        : m_type(Type::Literal), m_value(std::move(rvalue)) {}

		explicit AstNode(const Token & token, Var && value = Var());

		String toString();
		friend String & operator<<(String & string, const AstNode & self);

		friend ostream & operator<<(ostream & stream, const AstNode & self);
		static const cachar_t * typeName(Type type);
		meta_inline const cachar_t * typeName() const {
		    return typeName(m_type);
		}
		static Type forType(const Type & varKeyword, const Token::Type & inOrOf);
	};

	using AT =  AstNode::Type;
	using Ast         = Tree<AstNode>;
	using AstChildren = Ast::children_vector;


	namespace ForVanilla {
        enum {Condition = 0, Body, Init, EndExpr };
        inline Ast & condtion(Ast & forAst){
            Debug_suppose(forAst->is(AT::ForVanilla));
            return forAst[int(Condition)];
        }
        inline Ast & body(Ast & forAst){
            Debug_suppose(forAst->is(AT::ForVanilla));
            return forAst[int(Body)];
        }
        inline Ast & init(Ast & forAst){
            Debug_suppose(forAst->is(AT::ForVanilla));
            return forAst[int(Init)];
        }
        inline bool hasEndExpr(Ast & forAst){
            Debug_suppose(forAst->is(AT::ForVanilla));
            return int(EndExpr) < forAst.childrenSize();
        }
        inline Ast & endExpr(Ast & forAst){
            Debug_suppose(hasEndExpr(forAst));
            return forAst[int(EndExpr)];
        }

	}

	namespace Dot {
	    enum {Target = 0, Propname};
        inline Ast & target(Ast & dotAst){
            Debug_suppose(dotAst->is(AT::Dot) || dotAst->is(AT::CompMemberAccess));
	        return dotAst[int(Target)];
	    }
        inline Ast & propname(Ast & dotAst){
            Debug_suppose(dotAst->is(AT::Dot) || dotAst->is(AT::CompMemberAccess));
            return dotAst[int(Propname)];
	    }
	}
	namespace CMA = Dot;

	namespace Switch {
		enum {Target = 0, StatementPack, Cases};
        inline bool hasDefault(const Ast & switchAst){
			Debug_suppose(switchAst->type() == AT::Switch);
			return switchAst->value().isNumber();
		}
        inline size_t defaultStIndex(const Ast & switchAst){
			Debug_suppose(hasDefault(switchAst));
			return static_cast<size_t>(+switchAst->value().number());
		}
        inline size_t caseStIndex(const Ast & caseAst){
			Debug_suppose(caseAst->type() == AT::Case);
			return static_cast<size_t>(+caseAst->value().number());
		}
        inline Ast & stpack(Ast & switchAst){
			Debug_suppose(switchAst->type() == AT::Switch);
			return switchAst[int(StatementPack)];
		}
        inline Ast & target(Ast & switchAst){
			Debug_suppose(switchAst->type() == AT::Switch);
			return switchAst[int(Target)];
		}
        inline Ast & cases(Ast & switchAst){
			Debug_suppose(switchAst->type() == AT::Switch);
			return switchAst[int(Cases)];
		}
        inline bool hasTrailingLabels(const Ast & switchAst){
			Debug_suppose(switchAst->type() == AT::Switch);
			if (hasDefault(switchAst)
				&& defaultStIndex(switchAst) >= switchAst[int(StatementPack)].childrenSize()) {
				return true;
			}
			if (!switchAst[int(Cases)].isLeaf()) {
				return caseStIndex(switchAst[int(Cases)][0])
					 < switchAst[int(StatementPack)].childrenSize();
			}
			return false;
		}
	}

}