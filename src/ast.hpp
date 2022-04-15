#ifndef AST_HPP
#define AST_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "aliases.hpp"

namespace eris
{   
    enum class ASTType
    {
        NumericLiteral,
        StringLiteral,
        BlockStatement,
        ExpressionStatement,
    };

    class AST
    {
    public:
        int line;
        virtual ~AST() = default;
        virtual ASTType type() const = 0;
        virtual std::string str() = 0;
    };

    class NumericLiteralAST : public AST
    {
    public:
        double value;

        NumericLiteralAST(double value) : value(value) {}

        ASTType type() const
        {
            return ASTType::NumericLiteral;
        }

        std::string str()
        {
            return std::to_string(value);
        }
    };

    class StringLiteralAST : public AST
    {
    public:
        std::string string;

        StringLiteralAST(const std::string &string) : string(string) {}

        ASTType type() const
        {
            return ASTType::StringLiteral;
        }

        std::string str()
        {
            return string;
        }
    };

    class BlockStatementAST : public AST
    {
    public:
        std::vector<sh_ptr<AST> > body;

        BlockStatementAST(const std::vector<sh_ptr<AST> > &body)
            : body(body)
        {
        }

        ASTType type() const
        {
            return ASTType::BlockStatement;
        }

        std::string str()
        {
            std::string result = "BlockStatement(";
            for (std::size_t i = 0; i < body.size(); i++)
            {
                result += body.at(i)->str() + ",";
            }

            result.pop_back();

            result += ")";

            return result;
        }
    };

    class ExpressionStatementAST : public AST
    {
    public:
        sh_ptr<AST> expression;

        ExpressionStatementAST(sh_ptr<AST> expression)
            : expression(expression)
        {
        }

        ASTType type() const
        {
            return ASTType::ExpressionStatement;
        }

        std::string str()
        {
            return "ExpressionStatement(" + expression->str() + ")";
        }
    };
}

#endif