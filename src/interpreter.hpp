#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "values.hpp"

namespace eris
{
    /**
     * Eris interpreter.
     */
    class Interpreter
    {
    public:
        std::shared_ptr<Value> eval(std::shared_ptr<AST> exp)
        {
            return eval(exp.get());
        }

        std::shared_ptr<Value> eval(AST *exp)
        {
            switch (exp->type())
            {
            case ASTType::StatementList:
                return eval(dynamic_cast<StatementListAST *>(exp));
            case ASTType::ExpressionStatement:
                return eval(dynamic_cast<ExpressionStatementAST *>(exp));
            case ASTType::NumericLiteral:
                return eval(dynamic_cast<NumericLiteralAST *>(exp));
            case ASTType::StringLiteral:
                return eval(dynamic_cast<StringLiteralAST *>(exp));
            default:
                throw std::string("Unimplemented");
                return std::shared_ptr<Value>();
            }
        }

        std::shared_ptr<Value> eval(StatementListAST *exp)
        {
            for (std::size_t i = 0; i < exp->statementList.size() - 1; i++)
            {
                eval(exp->statementList.at(i));
            }

            return eval(exp->statementList.back());
        }

        std::shared_ptr<Value> eval(ExpressionStatementAST *exp)
        {
            return eval(exp->expression);
        }

        std::shared_ptr<Value> eval(NumericLiteralAST *exp)
        {
            return std::shared_ptr<Number>(new Number(exp->value));
        }

        std::shared_ptr<Value> eval(StringLiteralAST *exp)
        {
            return std::shared_ptr<String>(new String(exp->string));
        }
    };
}

#endif