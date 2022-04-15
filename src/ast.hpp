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
        EmptyStatement,
        BlockStatement,
        ExpressionStatement,
        NumericLiteral,
        StringLiteral
    };


    class AST
    {
    public:
        int line;
        virtual ~AST() = default;
        virtual ASTType type() const = 0;
        virtual std::string str() = 0;
    };


    class EmptyStatementAST : public AST
    {
    public:
        EmptyStatementAST(int line)
        {
            this->line = line;
        }

        ASTType type() const
        {
            return ASTType::EmptyStatement;
        }

        std::string str()
        {
            return "EmptyStatement()";
        }
    };


    class BlockStatementAST : public AST
    {
    public:
        std::vector<sh_ptr<AST> > body;

        BlockStatementAST(int line, const std::vector<sh_ptr<AST> > &body)
            : body(body)
        {
            this->line = line;
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

        ExpressionStatementAST(int line, sh_ptr<AST> expression)
            : expression(expression)
        {
            this->line = line;
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

    class NumericLiteralAST : public AST
    {
    public:
        double value;

        NumericLiteralAST(int line, double value)
            : value(value)
        {
            this->line = line;
        }

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

        StringLiteralAST(int line, const std::string &string)
            : string(string)
        {
            this->line = line;
        }

        ASTType type() const
        {
            return ASTType::StringLiteral;
        }

        std::string str()
        {
            return string;
        }
    };
}

#endif