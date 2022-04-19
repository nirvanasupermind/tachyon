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
        IfStatement,
        VariableStatement,
        EmptyStatement,
        BlockStatement,
        ExpressionStatement,
        AssignmentExpression,
        Identifier,
        BinaryExpression,
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

    class IfStatementAST : public AST
    {
    public:
        sh_ptr<AST> test;
        sh_ptr<AST> consequent;
        sh_ptr<AST> alternate;

        IfStatementAST(int line, sh_ptr<AST> test, sh_ptr<AST> consequent, sh_ptr<AST> alternate)
            : test(test), consequent(consequent), alternate(alternate)
        {
            this->line = line;
        }

        ASTType type() const
        {
            return ASTType::IfStatement;
        }

        std::string str()
        {
            return "IfStatement(" + test->str() + "," + consequent->str() + (alternate ? "," + alternate->str() : "") + ")";
        }
    };

    class VariableStatementAST : public AST
    {
    public:
        std::string name;
        sh_ptr<AST> value;

        VariableStatementAST(int line, const std::string &name, sh_ptr<AST> value)
            : name(name), value(value)
        {
            this->line = line;
        }

        ASTType type() const
        {
            return ASTType::VariableStatement;
        }

        std::string str()
        {
            return "VariableStatement(" + name + "," + value->str() + ")";
        }
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

    class AssignmentExpressionAST : public AST
    {
    public:
        std::string op;
        std::shared_ptr<AST> left;
        std::shared_ptr<AST> right;

        AssignmentExpressionAST(int line, const std::string &op, std::shared_ptr<AST> left, std::shared_ptr<AST> right)
            : op(op), left(left), right(right)
        {
            this->line = line;
        }

        ASTType type() const
        {
            return ASTType::AssignmentExpression;
        }

        std::string str()
        {
            return "AssignmentExpression("+op+","+left->str()+","+right->str()+")";
        }
    };

    class IdentifierAST : public AST
    {
    public:
        std::string name;

        IdentifierAST(int line, const std::string &name)
            : name(name)
        {
            this->line = line;
        }

        ASTType type() const
        {
            return ASTType::Identifier;
        }

        std::string str()
        {
            return name;
        }
    };

    class BinaryExpressionAST : public AST
    {
    public:
        std::string op;
        std::shared_ptr<AST> left;
        std::shared_ptr<AST> right;

        BinaryExpressionAST(int line, const std::string &op, std::shared_ptr<AST> left, std::shared_ptr<AST> right)
            : op(op), left(left), right(right)
        {
            this->line = line;
        }

        ASTType type() const
        {
            return ASTType::BinaryExpression;
        }

        std::string str()
        {
            return "BinaryExpression("+op+","+left->str()+","+right->str()+")";
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