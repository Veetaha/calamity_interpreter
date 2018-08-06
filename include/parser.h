#pragma once

#include <memory>
#include "meta.h"
#include "lexer.h"
#include "defs.h"
#include "ast.h"
#include "token.h"
#include "not_null_ptr.h"

namespace Cala {

    class GrammaticalInfo {
        std::shared_ptr<String> m_code;
        Ast m_ast;

    public:
        GrammaticalInfo(const std::shared_ptr<String> & code, Ast && ast = Ast())
            : m_code(code), m_ast(std::move(ast)) {}

        GrammaticalInfo(std::shared_ptr<String> && code, Ast && ast = Ast())
            : m_code(std::move(code)), m_ast(std::move(ast)) {}

        GrammaticalInfo(LexicalInfo && lexicalInfo, Ast && ast = Ast())
            : m_code(std::move(lexicalInfo.code())), m_ast(std::move(ast)) {}

        GrammaticalInfo(const LexicalInfo & lexicalInfo, Ast && ast = Ast())
            : m_code(std::move(lexicalInfo.code())), m_ast(std::move(ast)) {}


        meta_inline const std::shared_ptr<String> & code() const &  { return m_code; }
        meta_inline const Ast & ast() const &                       { return m_ast;   }
        meta_inline Ast & ast() &                                   { return m_ast;   }
    };

    class Parser {
    public:
        /**
         * @brief Builds an abstract syntax tree for the given lexical info.
         * @param lexicalInfo Lexical info that contains lexical tokens and code
         * @return GrammaticalInfo with according Ast and code.
         * @throw Exception if any kind of grammatical mistake was detected.
         *
         * Beware that lexicalInfo is an rvalue, that is it's in an invalid
         * state after being used (it's code pointer is stolen for performance
         * and iterators validity purpose).
         */
        static GrammaticalInfo parse(LexicalInfo && lexicalInfo);
    private:
        const LexicalInfo & m_lexis;
        LexicalInfo::const_iterator m_iter;
        LexicalInfo::const_iterator m_endIter;

        Parser(const LexicalInfo & lexis)
            : m_lexis(lexis),
              m_iter(lexis.tokens().cbegin()),
              m_endIter(lexis.tokens().cend()) {}

        meta_inline bool isEoi() const {
            return m_iter == m_endIter;
        }
        meta_inline const String & code() const {
            return *m_lexis.code();
        }

        meta_inline void unacceptLastToken() {
            --m_iter;
        }

        Exception unexpectedSyntaxException(
            const cacstr_t & issueDescr = ca("invalid syntax"),
            const cacstr_t & failTips = ca("")
        );

        Exception unexpectedTokenException(
            const Token::Type & expectedType = Token::Type::Eoi,
            const cachar_t * const & failTips = ca("")
        );
        /**
         *  @brief Returns true if current token is of [delimiterType] and advances the iterator,
         *  otherwise does nothing and returns false, including when end of input was reached.
         */
        bool acceptDelimiter(const Token::Type & delimiterType);
        /**
         * @brief Accepts a delimiter, throws unexpectedTokenException() if acceptDelimiter() == false.
         */
        void expectDelimiter(
            const Token::Type & delimiterType,
            const cacstr_t & failTips = ca("")
        );

        /**
         * @brief If current token is of type [tokenType] returns Ast with AstNode
         * representing this token (not all tokens are representable, assertion is provided) and
         * advances iterator one step forward.
         * If token is not of type [tokenType], or end of input was reached,
         * returns invalid AstNode (i.e. with Unknown type).
         */
        AstNode accept(const Token::Type & tokenType);
        /**
         * @brief Returns accept(tokenType), but throws unexpectedTokenExcetion() if
         * this method return invalid AstNode, thus if it returns, it guarantees a valid AstNode.
         */
        AstNode expect(const Token::Type & tokenType, const cacstr_t & failTips = ca(""));

        /**
         * @brief Checks wether ast->isValid() and returns it, otherwise throws Exception with
         * failTips appended to its message.
         */
        meta_inline Ast && expect(
            Ast && ast,
            const cacstr_t & problemDescr = ca("invalid syntax"),
            const cacstr_t & failTips = ca("")
        );
        Ast & expect(
            Ast & ast,
            const cacstr_t & problemDescr = ca("invalid syntax"),
            const cacstr_t & failTips = ca("")
        );

        LexicalInfo::const_iterator findClosingParen();


        meta_inline void backtrack(const LexicalInfo::const_iterator & position){
            m_iter = position;
        }

        /* Ebnf rules implementation: */


        AstNode var_keyword();
        Ast calamity();

        Ast statement();

        Ast funcdecl();
        Ast func_params();
        Ast paramlist();
        Ast restparam();
        Ast init();

        Ast vardecl();
        Ast vardecl_ap();
        Ast alter_st();
        Ast else_st();

        Ast cycle_st();
        Ast while_st();
        Ast do_while_st();
        Ast for_st();
        Ast for_st_ap();
        Ast for_decl();
        Ast for_expr_init_st();
        Ast for_inof_st(Ast & varKeyword, const Ast & varName);
        Ast for_decl_init_st(Ast && varKeyword, Ast && varName);
        Ast for_init_st_ap(Ast && initAst);
        meta_inline Ast for_no_init_st() {
            return for_init_st_ap(Ast());
        }


        Ast blank_st();
        Ast expression_st();
        Ast return_st();
        Ast throw_st();
        Ast try_catch_st();
        Ast switch_st();

        void addCaseToSwitch(Ast & switchAst, Ast && caseAst);
        void addDefaultToSwitch(Ast & switchAst);
        void addStatementToSwitch(Ast & switchAst, Ast && statementAst);

        void cases(Ast & switchAst);
        Ast case_lbl();
        bool default_lbl();

        Ast break_st();
        Ast continue_st();
        
        Ast block_st();
        Ast expression();

        Ast coma();
        Ast assign();
        Ast ternary_cond();
        Ast log_or();
        Ast log_and();
        Ast bit_or(); 
        Ast bit_xor(); 
        Ast bit_and(); 
        Ast equality(); 
        Ast compar_in(); 
        Ast bit_shift(); 
        Ast add(); 
        Ast mult(); 
        Ast exp(); 
        Ast prefix();
        Ast prefix_ap();
        Ast _delete();
        Ast postfix();
        Ast _new();
        Ast new_ap();
        Ast primary();
        Ast primary_operator(Ast & firstOp);
        Ast member_access(Ast & firstOp);
        Ast comp_member_access(Ast & firstOp);
        Ast function_call(Ast & firstOp);
        Ast parentheses(); 
        Ast parentheses_ap(); 
        Ast literal();
        Ast primitive_identifier();
        Ast string_or_number();
        Ast list_literal();
        Ast expr_list();
        Ast id_or_funclit();
        Ast lambda();
        Ast arrowfunc();
        Ast no_paren_arrowfunc();
        Ast arrowfunc_body();
        Ast paren_arrowfunc();
        Ast object_literal();
        Ast object_property();
        Ast object_property_ap();
        Ast object_propname();
        Ast object_comp_property();
        Ast identifier();

        using GrammarRule = Ast (Parser::*)();
        using TokTypes = std::initializer_list<Token::Type>;

        Ast expectOperand(const GrammarRule & operatorRule);
        Ast ltrBinaryOperator(
            const GrammarRule & higherRule,
            const TokTypes & operatorTTs
        );

        Ast rtlBinaryOperator(
            const GrammarRule & higherRule,
            const TokTypes & operatorTTs
        );

        template <typename TRule>
        Ast & applyMultiple(Ast & rootToAdd, const TRule & rule);
        template <typename TRule>
        meta_inline Ast && applyMultiple(Ast && rootToAdd, const TRule & rule);
        meta_inline const Substring & lastTokenName() const {
            Debug_suppose(m_iter > m_lexis.tokens().cbegin());
            return (m_iter - 1)->substr();
        }


        meta_inline Ast && firstValidAst(Ast && left, Ast && right){
            return static_cast<Ast &&>(left->isValid() ? left : right);
        }

        template <typename TRule, typename... TArgs>
        Ast selectRule(const std::initializer_list<TRule> & rules, TArgs &&... args);
        AstNode selectToken(const TokTypes & tokTypes);
        String traceStr(const Token::Type & expected) const;
        Token::Type currentTokenType() const;
    };


    meta_inline Ast && Parser::expect(Ast && ast,
        const cacstr_t & problemDescr,
        const cacstr_t & failTips){
        return expect(ast, problemDescr, failTips).rval();
    }


    template <typename TRule>
    Ast & Parser::applyMultiple(Ast & rootToAdd, const TRule & rule){
        Ast child;
        while ((child = (this->*rule)())->isValid()) {
            rootToAdd << child.rval();
        }
        return rootToAdd;
    }

    template <typename TRule>
    meta_inline Ast && Parser::applyMultiple(Ast && rootToAdd, const TRule & rule){
        return std::move(applyMultiple(rootToAdd, rule));
    }


    template <typename TRule, typename... TArgs>
    Ast Parser::selectRule(const std::initializer_list<TRule> & rules, TArgs &&... args){
        for (const TRule & rule : rules) {
            Ast ast((this->*rule)(std::forward<TArgs>(args)...));
            if (ast->isValid())
                return ast;
        }
        return Ast();
    }




}