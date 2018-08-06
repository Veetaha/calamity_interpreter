#include "ast.h"
#include "defs.h"

namespace Cala {
    namespace AstNodeTypename {
        static constexpr const cachar_t * const arrayMap[] = {
            ca("Unknown"),
            ca("Calamity"),
            ca("?:"),
            ca("&&"),
            ca("="),
            ca("+="),
            ca("-="),
            ca("*="),
            ca("/="),
            ca("%="),
            ca("**="),
            ca("&="),
            ca("|="),
            ca("^="),
            ca("<<="),
            ca(">>>="),
            ca(">>="),
            ca("&"),
            ca("|"),
            ca("^"),
            ca("<<"),
            ca(">>>"),
            ca(">>"),
            ca(","),
            ca("/"),
            ca("=="),
            ca(">="),
            ca(">"),
            ca("<="),
            ca("<"),
            ca("-"),
            ca("*"),
            ca("||"),
            ca("+"),
            ca("**"),
            ca("%"),
            ca("==="),
            ca("!="),
            ca("!=="),
            ca("in"),
            ca("instanceof"),
            ca("~"),
            ca("!"),
            ca("void"),
            ca("await"),
            ca("typeof"),
            ca("++pre"),
            ca("--pre"),
            ca("post++"),
            ca("post--"),
            ca("new"),
            ca("this"),
            ca("break"),
            ca("continue"),
            ca("Literal"),
            ca("Identifier"),
            ca("BlockSt"),
            ca("while"),
            ca("do_while"),
            ca("for(_;_;_)"),
            ca("for(let_in_)"),
            ca("for(var_in_)"),
            ca("for(const_in_)"),
            ca("for(let_of_)"),
            ca("for(var_of_)"),
            ca("for(const_of_)"),
            ca("let"),
            ca("const"),
            ca("var"),
            ca("Vardecl"),
            ca("ListLiteral"),
            ca("BlankSt"),
            ca("return"),
            ca("throw"),
            ca("try"),
            ca("switch"),
            ca("Cases"),
            ca("case"),
            ca("delete"),
            ca("Funcdecl"),
            ca("AsyncFuncdecl"),
            ca("GeneratorFuncdecl"),
            ca("AsyncGeneratorFuncdecl"),
            ca("Param"),
            ca("if"),
            ca("dot"),
            ca("Funclit"),
            ca("AsyncFunclit"),
            ca("GeneratorFunclit"),
            ca("AsyncGeneratorFunclit"),
            ca("Arrowfunc"),
            ca("Paramlist"),
            ca("obj[_]"),
            ca("RestParam"),
            ca("StatementPack"),
            ca("FnCall"),
            ca("ExprList"),
            ca("ObjLiteral"),
            ca("ObjProp")
        };
    }

    const cachar_t * AstNode::typeName(Type type) {
        using namespace AstNodeTypename;
        Debug_suppose(int(type) < Vtem::size(arrayMap));
        return arrayMap[static_cast<int>(type)];
    }


    String AstNode::toString() {
        String stringRepr;
        return stringRepr << *this;
    }

    String & operator<<(String & string, const AstNode & self) {
        string << self.name();
        if (self.name() != self.typeName()) {
            string.push_back(ca(" -> "), self.typeName());
        }
        if (!self.value().isUndefined()) {
            string << ca(", ");
            string << self.value();
        }
        return string;
    }

    ostream & operator<<(ostream & stream, const AstNode & self) {
        return stream
            << ca("{ ")
            << self.typeName()
            << ca(", ")
            << self.value()
            << ca(" }");
    }

    AstNode::AstNode(const Token & token, Var && value)
        : m_value(std::move(value)), m_name(token.substr()) {
        typedef Token::Type TokType;
        typedef AstNode::Type AstType;
        switch (token.type()) {
  //TODO:   case TokType::Enum:
            case TokType::Void:     { m_type = AstType::Void;      return; }
            case TokType::Try:      { m_type = AstType::Try;       return; }
//            case TokType::Catch:
            case TokType::Switch:   { m_type = AstType::Switch;    return; }
            case TokType::Case:     { m_type = AstType::Case;      return; }
            case TokType::This:     { m_type = AstType::This;      return; }
            case TokType::Break:    { m_type = AstType::Break;     return; }
            case TokType::Continue: { m_type = AstType::Continue;  return; }
            case TokType::Do:       { m_type = AstType::DoWhile;   return; }
            case TokType::In:       { m_type = AstType::In;        return; }
            case TokType::Delete:   { m_type = AstType::Delete;    return; }
            case TokType::Throw:    { m_type = AstType::Throw;     return; }
//            case TokType::Import:
//            case TokType::Export:
            case TokType::Typeof:   { m_type = AstType::Typeof;    return; }
//            case TokType::Yield:
            case TokType::Var:      { m_type = AstType::Var;       return; }
            case TokType::Let:      { m_type = AstType::Let;       return; }
            case TokType::Const:    { m_type = AstType::Const;     return; }
            case TokType::New:      { m_type = AstType::New;       return; }
            case TokType::False:    {
                m_type = AstType::Literal;
                m_value = false;
                return;
            }
//            case TokType::For:
//            case TokType::Function:
            case TokType::If:       { m_type = AstType::IfSt;      return; }
//            case TokType::Else:
            case TokType::True: {
                m_type = AstType::Literal;
                m_value = true;
                return;
            }                        // undefined is set by default
            case TokType::Undefined:{ m_type = AstType::Literal;  return; }
            case TokType::Null:{
                m_type = AstType::Literal;
                m_value = null;
                return;
            }
            case TokType::While:    { m_type = AstType::While;    return; }
            case TokType::Return:   { m_type = AstType::Return;   return; }
//            case TokType::Async:
            case TokType::Await:    { m_type = AstType::Await;    return; }
//            case TokType::Of:
            case TokType::Endl: {
                m_type = AstType::Literal;
                m_value = endl;
                return;
            }
            case TokType::Infinity:{
                m_type = AstType::Literal;
                m_value = Infinity;
                return;
            }
            case TokType::NaN:{
                m_type = AstType::Literal;
                m_value = NaN;
                return;
            }
            case TokType::Instanceof:      { m_type = AstType::Instanceof;       return; }
            case TokType::Identity:        { m_type = AstType::Identity;         return; }
            case TokType::NotIdentity:     { m_type = AstType::NotIdentity;      return; }
            case TokType::Increment:       { m_type = AstType::PreIncrement;     return; }
            case TokType::Decrement:       { m_type = AstType::PreDecrement;     return; }
            case TokType::LogAnd:          { m_type = AstType::LogAnd;           return; }
            case TokType::Assign:          { m_type = AstType::Assign;           return; }
            case TokType::PlusAssign:      { m_type = AstType::PlusAssign;       return; }
            case TokType::MinusAssign:     { m_type = AstType::MinusAssign;      return; }
            case TokType::MultiplyAssign:  { m_type = AstType::MultiplyAssign;   return; }
            case TokType::DivideAssign:    { m_type = AstType::DivideAssign;     return; }
            case TokType::RemainderAssign: { m_type = AstType::RemainderAssign;  return; }
            case TokType::PowerAssign:     { m_type = AstType::PowerAssign;      return; }
            case TokType::BitAndAssign:    { m_type = AstType::BitAndAssign;     return; }
            case TokType::BitOrAssign:     { m_type = AstType::BitOrAssign;      return; }
            case TokType::BitXorAssign:    { m_type = AstType::BitXorAssign;     return; }
            case TokType::BitLShiftAssign: { m_type = AstType::BitLShiftAssign;  return; }
            case TokType::Bit3RShiftAssign:{ m_type = AstType::Bit3RShiftAssign; return; }
            case TokType::Bit2RShiftAssign:{ m_type = AstType::Bit2RShiftAssign; return; }
            case TokType::BitAnd:          { m_type = AstType::BitAnd;           return; }
            case TokType::BitOr:           { m_type = AstType::BitOr;            return; }
            case TokType::BitNot:          { m_type = AstType::BitNot;           return; }
            case TokType::BitXor:          { m_type = AstType::BitXor;           return; }
            case TokType::BitLeftShift:    { m_type = AstType::BitLeftShift;     return; }
            case TokType::Bit3RightShift:  { m_type = AstType::Bit3RightShift;   return; }
            case TokType::Bit2RightShift:  { m_type = AstType::Bit2RightShift;   return; }
//            case TokType::Coma:
            case TokType::Division:        { m_type = AstType::Division;         return; }
            case TokType::Equality:        { m_type = AstType::Equality;         return; }
            case TokType::GreaterOrEqual:  { m_type = AstType::GreaterOrEqual;   return; }
            case TokType::GreaterThan:     { m_type = AstType::GreaterThan;      return; }
            case TokType::LessOrEqual:     { m_type = AstType::LessOrEqual;      return; }
            case TokType::LessThan:        { m_type = AstType::LessThan;         return; }
            case TokType::Minus:           { m_type = AstType::Minus;            return; }
            case TokType::Asterisk:        { m_type = AstType::Multiplication;   return; }
            case TokType::NotEquality:     { m_type = AstType::NotEquality;      return; }
            case TokType::LogNot:          { m_type = AstType::LogNot;           return; }
            case TokType::LogOr:           { m_type = AstType::LogOr;            return; }
            case TokType::Plus:            { m_type = AstType::Plus;             return; }
            case TokType::Power:           { m_type = AstType::Power;            return; }
//            case TokType::Ellipsis:
            case TokType::Remainder:       { m_type = AstType::Remainder;        return; }
            case TokType::Semicolon:       { m_type = AstType::BlankSt;          return; }
            case TokType::Dot:             { m_type = AstType::Dot;              return; }
            case TokType::QuestionMark:    { m_type = AstType::TernaryCondition; return; }
            case TokType::Identifier:      { m_type = AstType::Identifier;       return; }
            case TokType::String:          {
                m_type = AstType::Literal;
                m_value = token.substr().toString();
                return;
            }
            case TokType::Number: {
                m_type = AstType::Literal;
                m_value = Number::parse(token.substr());
                return;
            }
            default: {
                Debug_noimpl();
            }
        }
    }

    AstNode::Type AstNode::forType(const AstNode::Type & varKeyword, const Token::Type & inOrOf) {
        switch (varKeyword){
            case Type::Let: {
                return inOrOf == Token::Type::In ? Type::ForLetIn : Type::ForLetOf;
            }
            case Type::Var:{
                return inOrOf == Token::Type::In ? Type::ForVarIn : Type::ForVarOf;
            }
            case Type::Const: {
                return inOrOf == Token::Type::In ? Type::ForConstIn : Type::ForConstOf;
            }
            default:{ return Type::Unknown; }
        }
    }


}

