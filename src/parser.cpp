#include <parser.h>

#include "ast.h"
#include "parser.h"
#include "defs.h"

//#define NO_DEBUG_TRACE
#ifndef NO_DEBUG_TRACE
#define PARSER_TRACE_CALL() TRACE_CALL(Token::typeName(currentTokenType()));
#else
    #define PARSER_TRACE_CALL()
#endif

#define __OPTIONAL_CHILD(AST, ROOT_TO_ADD)({             \
    if ((AST)->isValid()) (ROOT_TO_ADD) << (AST).rval(); \
})
#define ACCEPT_AST(AST)({                 \
    if (!(AST)->isValid()) return Ast();  \
})
#define OPTIONAL_CHILD(AST, ROOT_TO_ADD)({          \
    if (Ast ___astOpt((AST)); ___astOpt->isValid()) \
       (ROOT_TO_ADD) << ___astOpt.rval();           \
})


#define ACCEPT_DELIMITER(DELIMITER) if (!acceptDelimiter(DELIMITER)) return Ast();


namespace Cala {

    typedef AstNode::Type AT;
    typedef Token::Type TT;
    
    

    Token::Type Parser::currentTokenType() const {
        return isEoi() ? TT::Eoi : m_iter->type();
    }

    String Parser::traceStr(const Token::Type & expected) const {
        String traceString;
        return traceString.push_back(
            ca("EXPECTED -> "), Token::typeName(expected),
            ca(" ACTUAL -> "), Token::typeName(currentTokenType())
        ).rval();
    }

    static String & addTips(String & dest, const cacstr_t & failTips) {
        return (*failTips) ? dest.push_back(ca("\nTip: "), failTips, ca(".")) : dest;
    }


    static String & addPoint(String & string, const Point2D<size_t> & point) {
        return string.push_back(ca('('), point.y, ca(", "), point.x, ca(')'));
    }

    static String & addCodeline(
        String & dest,
        const Substring & errorLine,
        const Point2D<size_t> & issuePoint
    ) {
        String temp;
        temp.push_back(ca('['), issuePoint.y, ca("] "));
        dest.push_back(temp, errorLine, endl);
        dest.append(temp.size() + issuePoint.x - 3, ca(' '));
        dest << ca("~~^");
        return dest;
    }

    Exception Parser::unexpectedSyntaxException(
        const cacstr_t & issueDescr,
        const cacstr_t & failTips
    ) {
        String errString;
        errString.push_back(ca("Unresolved parsing-time problem: "), issueDescr, ca("."));
        if (isEoi()) {
            errString.push_back(
                ca("\nParsing terminated at the end of input.")
            );
        } else {
            errString.push_back(ca("\nParsing terminated at token ["), m_iter->typeName(), ca("] at "));
            const auto issueIter(m_iter->substr().begin());
            const auto issuePoint(code().point2DOf(issueIter));
            addPoint(errString, issuePoint) << ca(".") << endl;
            addCodeline(errString, code().lineOf(issueIter), issuePoint);
        }
        addTips(errString, failTips);
        return Exception(errString.rval());
    }


    template<typename TStrOrSubstr, typename TStrOrSubstr2>
    static String & addExpected(
        String & dest,
        const TStrOrSubstr & expected,
        const TStrOrSubstr2 & actual
    ) {
        return dest.push_back(
            ca("expected token ["), expected, ca("] but experienced ["), actual, ca("]")
        );
    }

    Exception Parser::unexpectedTokenException(
        const Token::Type & expectedType,
        const cacstr_t & failTips
    ) {
        String errString(ca("Unresolved parsing-time problem: "));
        if (isEoi()) {
            addExpected(errString, Token::typeName(expectedType), Token::typeName(TT::Eoi));
            addTips(errString, failTips);
            return Exception(errString.rval());
        }
        const auto errBeginIter(m_iter->substr().begin());
        Point2D<size_t> mistakePoint(code().point2DOf(errBeginIter));
        addExpected(errString, Token::typeName(expectedType), m_iter->substr())
            << ca(" at ");
        addPoint(errString, mistakePoint)
            << endl;
        addCodeline(errString, code().lineOf(errBeginIter), mistakePoint);
        addTips(errString, failTips);
        return Exception(errString.rval());
    }

    bool Parser::acceptDelimiter(const Token::Type & delimiterType) {
        return ! isEoi() && delimiterType == m_iter->type()
               ? ++ m_iter, true
               : false;
    }


    void Parser::expectDelimiter(const Token::Type & expected, const cacstr_t & failTips) {
#ifndef NO_DEBUG_TRACE
        TRACE_CALL(traceStr(expected))
#endif
        if (! acceptDelimiter(expected)) {
            throw unexpectedTokenException(expected, failTips);
        }
    }


    AstNode Parser::accept(const Token::Type & tokenType) {
        return ! isEoi() && tokenType == m_iter->type()
               ? AstNode(*(m_iter ++))
               : AstNode();
    }


    AstNode Parser::expect(const Token::Type & expected, const cacstr_t & failTips) {
#ifndef NO_DEBUG_TRACE
        TRACE_CALL(traceStr(expected));
#endif
        if (AstNode accepted(accept(expected)); accepted.isValid())
            return accepted;
        else throw unexpectedTokenException(expected, failTips);
    }

    Ast & Parser::expect(Ast & ast, const cacstr_t & problemDescr, const cacstr_t & failTips) {
#ifndef NO_DEBUG_TRACE
        TRACE_CALL("EXPECTING VALID AST");
#endif
        if (ast->isValid()) return ast;
        else throw unexpectedSyntaxException(problemDescr, failTips);
    }

    AstNode Parser::selectToken(const TokTypes & tokTypes) {
        AstNode astNode;
        for (const TT & tokType : tokTypes) {
            if ((astNode = accept(tokType)).isValid()) {
                return astNode;
            }
        }
        return astNode;
    }


    GrammaticalInfo Parser::parse(LexicalInfo && lexicalInfo) {
        Parser parser(lexicalInfo);
        Ast resAst(parser.calamity());
        if (parser.isEoi()) {
            return GrammaticalInfo(std::move(lexicalInfo), std::move(resAst));
        } else {
            throw parser.unexpectedTokenException();
        }
    }


    Ast Parser::calamity() {    PARSER_TRACE_CALL();
        return applyMultiple(Ast((AstNode(AT::Calamity))), &Parser::statement);
    }

    Ast Parser::statement() {    PARSER_TRACE_CALL();
        //
        return selectRule({
            &Parser::funcdecl,
            &Parser::vardecl,
            &Parser::alter_st,
            &Parser::cycle_st,
            &Parser::block_st,
            &Parser::blank_st,
            &Parser::expression_st,
            &Parser::return_st,
            &Parser::throw_st,
            &Parser::try_catch_st,
            &Parser::switch_st,
            &Parser::break_st,
            &Parser::continue_st
        });
    }
    Ast Parser::funcdecl() {    PARSER_TRACE_CALL();
        Ast funcAst((AstNode(AT::Funcdecl)));
        if (acceptDelimiter(TT::Async)) {
            expectDelimiter(TT::Function);
            funcAst->setType(
                acceptDelimiter(TT::Asterisk)
                ? AT::AsyncGeneratorFuncdecl
                : AT::AsyncFuncdecl
            );
        } else {
            ACCEPT_DELIMITER(TT::Function);
            if (acceptDelimiter(TT::Asterisk)) {
                funcAst->setType(AT::GeneratorFuncdecl);
            }
        }
        funcAst->setName(expect(TT::Identifier).name());
        funcAst << expect(func_params(), ca("expected function parameter list"));
        funcAst << expect(
            block_st(),
            ca("expected function body"),
            ca("function body must be a block statement")
        );
        return funcAst;
    }

    Ast Parser::func_params() {    PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::LeftPar);
        auto leftParEnd(lastTokenName().end());
        Ast paramlistAst(paramlist());
        expectDelimiter(TT::RightPar,
            !paramlistAst.isLeaf() && paramlistAst.lastChild()->is(AT::RestParam)
                ? ca("rest parameter must be the last one")
                : ca("")
        );
        paramlistAst->setName(Substring(leftParEnd, lastTokenName().begin()));
        return paramlistAst;
    }

    // trailing comas are allowed
    Ast Parser::paramlist() {    PARSER_TRACE_CALL();
        Ast paramlistAst((AstNode(AT::Paramlist)));
        local_raii(paramlistAst->setName(
            paramlistAst.isLeaf()
            ? lastTokenName()
            : Substring(
                paramlistAst[0]->name().begin(),
                paramlistAst.lastChild()->name().end()
            )
        ));

        do {
            Ast paramAst(restparam());
            if (paramAst->isValid()) {
                acceptDelimiter(TT::Coma);  // accepting possible trailing coma
                paramlistAst << paramAst.rval();
                return paramlistAst;
            }
            *paramAst = accept(TT::Identifier); // identifier is optional, since there may be
            if (!paramAst->isValid())           // no params or there is a trailing coma
                return paramlistAst;

            paramAst->setType(AT::Param);
            OPTIONAL_CHILD(init(), paramAst);
            paramlistAst << paramAst.rval();
        } while (acceptDelimiter(TT::Coma));
        return paramlistAst;
    }

    Ast Parser::init() {    PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::Assign);
        return expect(assign(), ca("expected init expression"));
    }

    Ast Parser::restparam() {    PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::Ellipsis);
        Ast restParamAst(expect(TT::Identifier, ca("expected rest parameter name")));
        restParamAst->setType(AT::RestParam);
        return restParamAst;
    }

    AstNode Parser::var_keyword() {    PARSER_TRACE_CALL();
        return selectToken({ TT::Let, TT::Var, TT::Const });
    }

    Ast Parser::vardecl() {    PARSER_TRACE_CALL();
        Ast varAst(var_keyword());
        ACCEPT_AST(varAst);
        Ast vardeclAst(expect(TT::Identifier));
        vardeclAst->setType(AT::Vardecl);
        OPTIONAL_CHILD(init(), vardeclAst);
        varAst << vardeclAst.rval();
        applyMultiple(varAst, &Parser::vardecl_ap);
        expectDelimiter(TT::Semicolon);
        return varAst;
    }

    Ast Parser::vardecl_ap() {    PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::Coma);
        Ast vardeclAst(expect(TT::Identifier));
        vardeclAst->setType(AT::Vardecl);
        OPTIONAL_CHILD(init(), vardeclAst);
        return vardeclAst;
    }
    Ast Parser::block_st() {    PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::LeftCurly);
        Ast blockstAst((AstNode(AT::BlockSt, lastTokenName())));
        applyMultiple(blockstAst, &Parser::statement);
        expectDelimiter(TT::RightCurly);
        return blockstAst;
    }
    Ast Parser::alter_st() {    PARSER_TRACE_CALL();
        Ast ifAst(accept(TT::If));
        ACCEPT_AST(ifAst);
        expectDelimiter(TT::LeftPar);
        ifAst << expect(expression(), ca("expected condition for if statement"));
        expectDelimiter(TT::RightPar);
        ifAst << expect(statement(), ca("expected true branch statement"));
        OPTIONAL_CHILD(else_st(), ifAst);
        return ifAst;
    }
    Ast Parser::else_st(){    PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::Else);
        return expect(statement(), ca("expected false branch statement"));
    }
    Ast Parser::cycle_st(){    PARSER_TRACE_CALL();
        return selectRule({&Parser::while_st, &Parser::do_while_st, &Parser::for_st});
    }
    Ast Parser::while_st(){    PARSER_TRACE_CALL();
        Ast whileAst(accept(TT::While));
        ACCEPT_AST(whileAst);
        expectDelimiter(TT::LeftPar);
        whileAst << expect(expression(), ca("expected while cycle condition expression"));
        expectDelimiter(TT::RightPar);
        whileAst << expect(statement(), ca("expected while cycle body statement"));
        return whileAst;
    }
    Ast Parser::do_while_st(){    PARSER_TRACE_CALL();
        Ast doWhileAst(accept(TT::Do));
        ACCEPT_AST(doWhileAst);
        Ast bodyAst(expect(statement(), ca("expected do-while cycle body statement")));
        expectDelimiter(TT::While);
        expectDelimiter(TT::LeftPar);
        doWhileAst << expect(expression(), ca("expected do-while cycle condition expression"));
        doWhileAst << bodyAst.rval();
        expectDelimiter(TT::RightPar);
        expectDelimiter(TT::Semicolon);
        return doWhileAst;
    }
    Ast Parser::for_st(){    PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::For);
        Substring forName(lastTokenName());
        expectDelimiter(TT::LeftPar);
        Ast forAst(expect(for_st_ap(), ca("expected variable declaration or [;]")));
        if (forAst->is(AT::ForVanilla)){
            forAst->setName(forName);
        }
        return forAst;
    }
    Ast Parser::for_st_ap(){    PARSER_TRACE_CALL();
        return selectRule({
            &Parser::for_decl, & Parser::for_expr_init_st
        });
    }
    Ast Parser::for_decl(){    PARSER_TRACE_CALL();
        Ast varKeywordAst(var_keyword());
        ACCEPT_AST(varKeywordAst);
        Ast varName(expect(TT::Identifier));
        Ast forInOfAst(for_inof_st(varKeywordAst, varName));
        return forInOfAst->isValid()
            ? forInOfAst.rval()
            : for_decl_init_st(varKeywordAst.rval(), varName.rval());
    }
    Ast Parser::for_expr_init_st(){
        Ast exprInit(expression());
        if (!exprInit->isValid()){
            exprInit->setType(AT::Literal); // undefined is by default
            exprInit->setName(lastTokenName());
        }
        return for_init_st_ap(exprInit.rval());
    }
    Ast Parser::for_inof_st(Ast & varKeyword, const Ast & varName){    PARSER_TRACE_CALL();
        Ast forAst((AstNode(AstNode::forType(varKeyword->type(), currentTokenType()))));
        ACCEPT_AST(forAst);
        forAst << (expect(expression(), ca("expected cycle target iterable object")));
        expectDelimiter(TT::RightPar);
        forAst << (expect(statement(),  ca("expected for cycle body statement")));
        forAst->setName(varName->name());
        return forAst;
    }
    Ast Parser::for_decl_init_st(Ast && varKeyword, Ast && varName){    PARSER_TRACE_CALL();
        OPTIONAL_CHILD(init(), varName);
        varName->setType(AT::Vardecl);
        varKeyword << varName.rval();
        applyMultiple(varKeyword, &Parser::vardecl_ap);
        return for_init_st_ap(varKeyword.rval());
    }
    Ast Parser::for_init_st_ap(Ast && initAst){    PARSER_TRACE_CALL();
        Debug_suppose(initAst->isValid());
        static_assert(int(ForVanilla::Condition) == 0);
        static_assert(int(ForVanilla::Body)      == 1);
        static_assert(int(ForVanilla::Init)      == 2);
        static_assert(int(ForVanilla::EndExpr)   == 3);

        expectDelimiter(TT::Semicolon);
        Ast conditionAst(expression());
        if (!conditionAst->isValid()){
            conditionAst->setType(AT::Literal);
            conditionAst->setValue(true);
        }
        expectDelimiter(TT::Semicolon);
        Ast endAst = expression(); // optional end expression
        expectDelimiter(TT::RightPar);
        Ast forAst((AstNode(AT::ForVanilla)));
        forAst << conditionAst.rval();
        forAst << expect(statement(), ca("expected for cycle body statement"));
        forAst << initAst.rval();
        __OPTIONAL_CHILD(endAst,  forAst);
        return forAst;
    }
    Ast Parser::blank_st(){    PARSER_TRACE_CALL();
        return Ast(accept(TT::Semicolon));
    }
    Ast Parser::expression_st(){    PARSER_TRACE_CALL();
        Ast exprAst(expression());
        ACCEPT_AST(exprAst);
        expectDelimiter(TT::Semicolon);
        return exprAst;
    }
    Ast Parser::return_st(){    PARSER_TRACE_CALL();
        Ast returnAst(accept(TT::Return));
        ACCEPT_AST(returnAst);
        Ast returnExpr(expression());
        if (!returnExpr->isValid()){
            returnExpr->setType(AT::Literal); // value is undefined by default
            returnExpr->setName(returnAst->name());
        }
        expectDelimiter(TT::Semicolon);
        returnAst << returnExpr.rval();
        return returnAst;
    }
    Ast Parser::throw_st(){    PARSER_TRACE_CALL();
        Ast throwAst(accept(TT::Throw));
        ACCEPT_AST(throwAst);
        throwAst << expect(expression(), ca("expected expression after [throw]"));
        return throwAst;
    }
    Ast Parser::try_catch_st(){    PARSER_TRACE_CALL();
        Ast tryAst(accept(TT::Try));
        ACCEPT_AST(tryAst);
        tryAst << expect(block_st(), ca("expected try block statement"));
        expectDelimiter(TT::Catch);
        expectDelimiter(TT::LeftPar);
        tryAst->setName(expect(TT::Identifier).name());
        expectDelimiter(TT::RightPar);
        tryAst << expect(block_st(), ca("expected catch block statement"));
        return tryAst;
    }
    Ast Parser::switch_st(){    PARSER_TRACE_CALL();
        static_assert(int(Switch::Target == 0));

        Ast switchAst(accept(TT::Switch));
        ACCEPT_AST(switchAst);
        expectDelimiter(TT::LeftPar);
        switchAst << expect(expression(), ca("expected switch target expression"));
        expectDelimiter(TT::RightPar);
        expectDelimiter(TT::LeftCurly);
        cases(switchAst);
        expectDelimiter(TT::RightCurly);
        return switchAst;
    }
    
    void Parser::addCaseToSwitch(Ast & switchAst, Ast && caseAst){
        Ast & casesAst( Switch::cases(switchAst));
        Ast & stpackAst(Switch::stpack(switchAst));
        
        caseAst->setValue(stpackAst.childrenSize());
        casesAst << caseAst.rval();
    }
    void Parser::addDefaultToSwitch(Ast & switchAst){
        Ast & stpackAst(Switch::stpack(switchAst));
        if (switchAst->value().isUndefined()) {
            switchAst->setValue(stpackAst.childrenSize());
        } else throw unexpectedSyntaxException(
            ca("switch statement with multiple default labels")
        );
    }

    void Parser::addStatementToSwitch(Ast & switchAst, Ast && statementAst){
        Ast & casesAst( Switch::cases(switchAst));
        Ast & stpackAst(Switch::stpack(switchAst));
        if (casesAst.isLeaf() || switchAst->value().isUndefined()){
            throw unexpectedSyntaxException(
                ca("leading statements in [switch-case] are prohibited")
            );
        }
        stpackAst << statementAst.rval();
    }

    void Parser::cases(Ast & switchAst){    PARSER_TRACE_CALL();
        static_assert(int(Switch::StatementPack == 1));
        static_assert(int(Switch::Cases == 2));

        switchAst << Ast(AstNode(AT::StatementPack));
        switchAst << Ast(AstNode(AT::Cases));
        for(;;) {
            if (Ast caseAst(case_lbl()); caseAst->isValid()){
                addCaseToSwitch(switchAst, caseAst.rval());
            } else if (default_lbl()){
                addDefaultToSwitch(switchAst);
            } else if (Ast statementAst(statement()); statementAst->isValid()){
                addStatementToSwitch(switchAst, statementAst.rval());
            } else if (Switch::hasTrailingLabels(switchAst)){
                throw unexpectedSyntaxException(
                    ca("expected statement after case or default label")    
                );
            } else return;
        }
    }
    Ast Parser::case_lbl(){    PARSER_TRACE_CALL();
        Ast caseAst(accept(TT::Case));
        ACCEPT_AST(caseAst);
        caseAst << expect(expression(), ca("expected case value expression"));
        expectDelimiter(TT::Colon);
        return caseAst;
    }
    bool Parser::default_lbl(){    PARSER_TRACE_CALL();
        if (acceptDelimiter(TT::Default)) {
            expectDelimiter(TT::Colon);
            return true;
        }
        else return false;
    }
    Ast Parser::break_st(){    PARSER_TRACE_CALL();
        Ast breakAst(accept(TT::Break));
        ACCEPT_AST(breakAst);
        expectDelimiter(TT::Semicolon);
        return breakAst;
    }
    Ast Parser::continue_st(){    PARSER_TRACE_CALL();
        Ast continueAst(accept(TT::Continue));
        ACCEPT_AST(continueAst);
        expectDelimiter(TT::Semicolon);
        return continueAst;
    }
    
    Ast Parser::expression() {    PARSER_TRACE_CALL();
        return coma();
    }
    Ast Parser::expectOperand(const GrammarRule & operatorRule){
        return expect((this->*operatorRule)(), ca("expected operand for operator"));
    }
    Ast Parser::rtlBinaryOperator(
        const GrammarRule & higherRule,
        const TokTypes & operatorTTs
    ){
        Ast higherRuleAst((this->*higherRule)());
        ACCEPT_AST(higherRuleAst);
        Ast topOpAst(selectToken(operatorTTs));
        Ast * prevOpAst = &topOpAst;
        while((*prevOpAst)->isValid()) {
            *prevOpAst << higherRuleAst.rval();
            higherRuleAst = expectOperand(higherRule);
            *prevOpAst << Ast(selectToken(operatorTTs));
            prevOpAst = prevOpAst->childPtr(1);
        }
        *prevOpAst = higherRuleAst.rval();
        return topOpAst;
    }
    Ast Parser::ltrBinaryOperator(
        const GrammarRule & higherRule,
        const TokTypes & operatorTTs
    ){
        Ast prevAst((this->*higherRule)());
        ACCEPT_AST(prevAst);
        for (Ast nextAst(selectToken(operatorTTs)); nextAst->isValid();) {
            nextAst << prevAst.rval() << expectOperand(higherRule);
            prevAst = nextAst.rval();
            *nextAst = selectToken(operatorTTs);
        }
        return prevAst;
    }
    Ast Parser::coma(){                  PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::assign, {TT::Coma});
    }
    Ast Parser::assign(){                PARSER_TRACE_CALL();
        return rtlBinaryOperator(&Parser::ternary_cond, {
           TT::Assign,          TT::PlusAssign,       TT::MinusAssign, TT::MultiplyAssign,
           TT::DivideAssign,    TT::RemainderAssign,  TT::PowerAssign,
           TT::BitOrAssign,     TT::BitAndAssign,     TT::BitXorAssign,
           TT::BitLShiftAssign, TT::Bit2RShiftAssign, TT::Bit3RShiftAssign
        });
    }
    Ast Parser::ternary_cond(){          PARSER_TRACE_CALL();
        Ast conditionAst(log_or());
        ACCEPT_AST(conditionAst);
        Ast ternaryAst(accept(TT::QuestionMark));
        if (!ternaryAst->isValid()){
            return conditionAst;
        }
        ternaryAst << conditionAst.rval();
        ternaryAst << expect(assign(), ca("expected ternary operator true case expression"));
        expectDelimiter(TT::Colon);
        ternaryAst << expect(assign(), ca("expected ternary operator false case expression"));
        return ternaryAst;
    }
    Ast Parser::log_or(){                PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::log_and, {TT::LogOr});
    }
    Ast Parser::log_and(){               PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::bit_or, {TT::LogAnd});
    }
    Ast Parser::bit_or(){                PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::bit_xor, {TT::BitOr});
    }
    Ast Parser::bit_xor(){               PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::bit_and, {TT::BitXor});
    }
    Ast Parser::bit_and(){               PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::equality, {TT::BitAnd});
    }
    Ast Parser::equality(){              PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::compar_in, {
            TT::Identity, TT::NotIdentity, TT::Equality, TT::NotEquality
        });
    }
    Ast Parser::compar_in(){             PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::bit_shift, {
           TT::LessThan, TT::GreaterThan, TT::LessOrEqual, TT::GreaterOrEqual,
           TT::In, TT::Instanceof
        });
    }
    Ast Parser::bit_shift(){             PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::add, {
           TT::BitLeftShift, TT::Bit2RightShift, TT::Bit3RightShift
        });
    }
    Ast Parser::add(){                   PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::mult, {
            TT::Plus, TT::Minus
        });
    }
    Ast Parser::mult(){                  PARSER_TRACE_CALL();
        return ltrBinaryOperator(&Parser::exp, {
            TT::Asterisk, TT::Division, TT::Remainder
        });
    }
    Ast Parser::exp(){                   PARSER_TRACE_CALL();
        return rtlBinaryOperator(&Parser::prefix, {TT::Power});
    }
    Ast Parser::prefix(){                PARSER_TRACE_CALL();
        return selectRule({
            &Parser::postfix, &Parser::_delete, &Parser::prefix_ap
        });
    }
    Ast Parser::prefix_ap(){             PARSER_TRACE_CALL();
        Ast prefixAst(selectToken({
            TT::Increment, TT::Decrement, TT::Plus, TT::Minus, TT::LogNot, TT::BitNot,
            TT::Await, TT::Typeof, TT::Void
        }));
        ACCEPT_AST(prefixAst);
        prefixAst << expect(prefix(), ca("expected operand after prefix unary operator"));
        return prefixAst;
    }
    Ast Parser::_delete(){
        Ast deleteAst(accept(TT::Delete));
        ACCEPT_AST(deleteAst);
        deleteAst << expect(postfix(), ca("expected target object after [delete] operator"));
        if (deleteAst[0]->is(AT::Dot)
         || deleteAst[0]->is(AT::CompMemberAccess)
         || deleteAst[0]->is(AT::Identifier)) {
            return deleteAst;
        } else throw unexpectedSyntaxException(
            ca("expected global property identifier or "
               "member access expression after [delete] operator")
        );
    }
    Ast Parser::postfix(){               PARSER_TRACE_CALL();
        Ast newAst(_new());
        ACCEPT_AST(newAst);
        if (Ast incrAst(accept(TT::Increment)); incrAst->isValid()){
            incrAst->setType(AT::PostIncrement);
            incrAst << newAst.rval();
            return incrAst;
        }
        if (Ast decrAst(accept(TT::Increment)); decrAst->isValid()){
            decrAst->setType(AT::PostDecrement);
            decrAst << newAst.rval();
            return decrAst;
        }
        return newAst;
    }
    Ast Parser::_new(){                  PARSER_TRACE_CALL();
        return selectRule({&Parser::primary, &Parser::new_ap});
    }
    Ast Parser::new_ap(){                PARSER_TRACE_CALL();
        Ast newAst(accept(TT::New));
        ACCEPT_AST(newAst);
        newAst << expect(_new(), ca("expected new or primary expression after [new] operator"));
        return newAst;
    }
    Ast Parser::primary(){               PARSER_TRACE_CALL();
        Ast parenAst(parentheses());
        ACCEPT_AST(parenAst);
        return firstValidAst(primary_operator(parenAst), parenAst.rval());
    }
    Ast Parser::primary_operator(Ast & firstOp){      PARSER_TRACE_CALL();
        return selectRule({
            &Parser::member_access, &Parser::comp_member_access, &Parser::function_call
        }, firstOp);
    }
    Ast Parser::member_access(Ast & firstOp){         PARSER_TRACE_CALL();
        static_assert(int(Dot::Target)   == 0);
        static_assert(int(Dot::Propname) == 1);

        Ast dotAst(accept(TT::Dot));
        ACCEPT_AST(dotAst);
        dotAst << firstOp.rval();
        dotAst << Ast(expect(TT::Identifier));
        return firstValidAst(primary_operator(dotAst), dotAst.rval());
    }
    Ast Parser::comp_member_access(Ast & firstOp){    PARSER_TRACE_CALL();
        static_assert(int(CMA::Target)   == 0);
        static_assert(int(CMA::Propname) == 1);
        ACCEPT_DELIMITER(TT::LeftBracket);
        Ast accessAst((AstNode(AT::CompMemberAccess)));
        accessAst << firstOp.rval();
        accessAst << expect(expression(), ca("expected member access expression"));
        expectDelimiter(TT::RightBracket);
        return firstValidAst(primary_operator(accessAst), accessAst.rval());
    }
    Ast Parser::function_call(Ast & firstOp){         PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::LeftPar);
        Ast fnCallAst((AstNode(AT::FnCall, firstOp->name())));
        fnCallAst << firstOp.rval();
        fnCallAst << expr_list();
        expectDelimiter(TT::RightPar);
        fnCallAst->name().setEnd(lastTokenName().end());
        fnCallAst[1]->setName(Substring(
            fnCallAst->name().begin(), lastTokenName().end()
        ));
        return firstValidAst(primary_operator(fnCallAst), fnCallAst.rval());
    }

    Ast Parser::parentheses(){           PARSER_TRACE_CALL();
        return selectRule({&Parser::literal, &Parser::parentheses_ap});
    }
    Ast Parser::parentheses_ap(){        PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::LeftPar);
        Ast exprAst(expect(expression(), ca("expected expression after [(]")));
        expectDelimiter(TT::RightPar);
        return exprAst;
    }
    Ast Parser::literal(){               PARSER_TRACE_CALL();
        return selectRule({
            &Parser::primitive_identifier,
            &Parser::string_or_number,
            &Parser::list_literal,
            &Parser::id_or_funclit,
            &Parser::object_literal
        });
    }
    Ast Parser::primitive_identifier(){      PARSER_TRACE_CALL();
        return Ast(selectToken({
            TT::True, TT::False, TT::Null, TT::Undefined,
            TT::NaN,  TT::Infinity, TT::Endl
        }));
    }
    Ast Parser::string_or_number(){          PARSER_TRACE_CALL();
        return Ast(selectToken({TT::String, TT::Number}));
    }
    Ast Parser::list_literal(){              PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::LeftBracket);
        Ast listAst((AstNode(AT::ListLiteral, lastTokenName())));
        listAst << expr_list();
        expectDelimiter(TT::RightBracket);
        listAst->name().setEnd(lastTokenName().end());
        listAst[0]->setName(listAst->name());
        return listAst;
    }
    Ast Parser::expr_list(){                 PARSER_TRACE_CALL();
        Ast exprListAst((AstNode(AT::ExprList)));
        for (Ast exprAst(assign()); exprAst->isValid(); exprAst = assign()){
            exprListAst << exprAst.rval();
            if (!acceptDelimiter(TT::Coma)){
                return exprListAst;
            }
        }
        return exprListAst;
    }
    Ast Parser::id_or_funclit(){          PARSER_TRACE_CALL();
        return selectRule({&Parser::lambda, &Parser::arrowfunc});
    }
    Ast Parser::lambda(){                     PARSER_TRACE_CALL();
        Ast lambdaAst((AstNode(AT::Funclit)));
        if (acceptDelimiter(TT::Async)) {
            expectDelimiter(TT::Function);
            lambdaAst->setType(
                acceptDelimiter(TT::Asterisk)
                ? AT::AsyncGeneratorFunclit
                : AT::AsyncFunclit
            );
        } else {
            ACCEPT_DELIMITER(TT::Function);
            if (acceptDelimiter(TT::Asterisk)) {
                lambdaAst->setType(AT::GeneratorFunclit);
            }
        }
        lambdaAst->setName(accept(TT::Identifier).name());
        lambdaAst << expect(func_params(), ca("expected function parameter list"));
        lambdaAst << expect(
            block_st(),
            ca("expected function body"),
            ca("function body must be a block statement")
        );
        return lambdaAst;
    }
    Ast Parser::arrowfunc(){                 PARSER_TRACE_CALL();
        return selectRule({
           &Parser::no_paren_arrowfunc, &Parser::paren_arrowfunc
        });
    }
    Ast Parser::no_paren_arrowfunc(){        PARSER_TRACE_CALL();
        Ast idAst(accept(TT::Identifier));
        ACCEPT_AST(idAst);
        Ast bodyAst(arrowfunc_body());
        if (!bodyAst->isValid()){
            return idAst;
        }
        Ast arrowAst((AstNode(AT::Arrowfunc, idAst->name())));
        arrowAst << idAst.rval();
        arrowAst << bodyAst.rval();
        return arrowAst;
    }
    Ast Parser::arrowfunc_body(){            PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::Arrow);
        return selectRule({&Parser::block_st, &Parser::assign});
    }
    Ast Parser::paren_arrowfunc(){           PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::LeftPar);
        auto closingPar(findClosingParen());
        if (closingPar == m_endIter){
            throw unexpectedSyntaxException(
                ca("expected closing parenthesis")
            );
        }
        unacceptLastToken(); // left paren is taken in func_params or parentheses rule
        if ((++closingPar)->is(TT::Arrow)) {
            Ast paramAst(expect(func_params()));
            Ast arrowAst((AstNode(AT::Arrowfunc, paramAst->name())));
            arrowAst << paramAst.rval();
            arrowAst << expect(arrowfunc_body(), ca("expected arrow function body"));
            return arrowAst;
        } else return Ast();
    }
    Ast Parser::object_literal(){            PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::LeftCurly);
        Ast objAst((AstNode(AT::ObjLiteral, lastTokenName())));
        Ast objFirstProp(object_property());
        if (objFirstProp->isValid()){
            applyMultiple(objAst, &Parser::object_property_ap);
        }
        expectDelimiter(TT::RightCurly);
        objAst->name().setEnd(lastTokenName().end());
        return objAst;
    }
    Ast Parser::object_property(){           PARSER_TRACE_CALL();
        Ast propNameAst(object_propname());
        ACCEPT_AST(propNameAst);
        Ast propAst((AstNode(AT::ObjProp, propNameAst->name())));
        propAst << propNameAst.rval();
        expectDelimiter(TT::Colon);
        propAst << expect(assign(), ca("expected expression after property name [:]"));
        return propAst;
    }
    Ast Parser::object_property_ap(){        PARSER_TRACE_CALL();
        ACCEPT_DELIMITER(TT::Coma);
        return expect(object_property(), ca("expected object property after [,]"));
    }
    Ast Parser::object_propname(){           PARSER_TRACE_CALL();
        return selectRule({
            &Parser::identifier,
            &Parser::string_or_number,
            &Parser::object_comp_property
        });
    }
    Ast Parser::object_comp_property(){
        ACCEPT_DELIMITER(TT::LeftBracket);
        Ast propExprAst(expect(expression(), ca(
            "expected expression after [[]"
        )));
        expectDelimiter(TT::RightBracket);
        return propExprAst;
    }

    Ast Parser::identifier(){                PARSER_TRACE_CALL();
        return Ast(accept(TT::Identifier));
    }

    LexicalInfo::const_iterator Parser::findClosingParen() {
        int openParens(1);
        for (auto iter(m_iter); iter != m_endIter; ++iter) {
            switch (iter->type()) {
                case TT::LeftPar: ++openParens;
                default: break;
                case TT::RightPar: if (!--openParens) return iter;
            }
        }
        return m_endIter;
    }







}
    // ---------------------------------------

/*
// C LEGACY:
    static Ast * Ebnf_apMainRule(__Parser * parser, GrammarRule higherRule, GrammarRule apRule) {
        Debug_exists(higherRule);
        Debug_exists(apRule);
        //
        Ast * leftOperand = higherRule(parser);
        CHECK(leftOperand);
        Ast * top = apRule(parser);
        if (top) {
            Ast * cur = top;
            while (Ast_childrenSize(cur) != 1) {
                cur = Ast_childAt(cur, 0);
            }
            Ast_addFirstChild(cur, leftOperand);
            return top;
        } else {
            CHECK(! parser->error, leftOperand);
            return leftOperand;
        }
    }

    static Ast * __Ebnf_apRule(__Parser * parser, GrammarRule higherRule,
                                LexemeType * lexems, unsigned char size) {
        Debug_exists(higherRule);
        Debug_exists(lexems);
        //
        Ast * _operator = __Ebnf_selectLexeme(parser, lexems, size);
        CHECK(_operator);

        Ast * operand = higherRule(parser);  // skazal A, skazhi B
        EXPECT_RULE(operand, _operator,
            "Expected expression after binary operator ");
        Ast_addFirstChild(_operator, operand);

        Ast * top = __Ebnf_apRule(parser, higherRule, lexems, size);
        if (top) {
            Ast * cur = top;
            while (Ast_childrenSize(cur) == 2) {
                cur = Ast_childAt(cur, 0);
            }
            Ast_addFirstChild(cur, _operator); // a and b are operands of higher precendece
            return top;
        } else {                               // [ ] - brackets define current function scope
            // case ... + a [ +  b  EPSILON ] - recursion delimiter
            CHECK(! parser->error, _operator);
            return _operator;
        }
    }
*/