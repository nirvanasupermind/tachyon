#ifndef AST_HPP
#define AST_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "aliases.hpp"

namespace eris
{
    class AST
    {
    public:
        int line;
        virtual ~AST() = default;
        virtual std::string type() const = 0;
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

        std::string type() const
        {
            return "NumericLiteral";
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

        std::string type() const
        {
            return "StringLiteral";
        }
    };

    class BooleanLiteralAST : public AST
    {
    public:
        bool value;

        BooleanLiteralAST(int line, bool value)
            : value(value)
        {
            this->line = line;
            this->value = value;
        }

        std::string type() const
        {
            return "BooleanLiteral";
        }
    };

    class NullLiteralAST : public AST
    {
    public:
        NullLiteralAST(int line)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "NullLiteral";
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

        std::string type() const
        {
            return "AssignmentExpression";
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

        std::string type() const
        {
            return "Identifier";
        }
    };

    class LogicalExpressionAST : public AST
    {
    public:
        std::string op;
        std::shared_ptr<AST> left;
        std::shared_ptr<AST> right;

        LogicalExpressionAST(int line, const std::string &op, std::shared_ptr<AST> left, std::shared_ptr<AST> right)
            : op(op), left(left), right(right)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "LogicalExpression";
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

        std::string type() const
        {
            return "BinaryExpression";
        }
    };


    // --------------------------------------------
    // Statements:

    class EmptyStatementAST : public AST
    {
    public:
        EmptyStatementAST(int line)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "EmptyStatement";
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

        std::string type() const
        {
            return "BlockStatement";
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

        std::string type() const
        {
            return "ExpressionStatement";
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

        std::string type() const
        {
            return "VariableStatement";
        }
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

        std::string type() const
        {
            return "IfStatement";
        }
    };

    class WhileStatementAST : public AST
    {
    public:
        sh_ptr<AST> test;
        sh_ptr<AST> body;

        WhileStatementAST(int line, sh_ptr<AST> test, sh_ptr<AST> body)
            : test(test), body(body)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "WhileStatement";
        }
    };

    class DoWhileStatementAST : public AST
    {
    public:
        sh_ptr<AST> body;
        sh_ptr<AST> test;

        DoWhileStatementAST(int line, sh_ptr<AST> body, sh_ptr<AST> test)
            :  body(body), test(test)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "DoWhileStatement";
        }
    };
}

#endif