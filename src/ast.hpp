#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

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

    // --------------------------------------------
    // Statements:

    class FunctionDeclarationAST : public AST
    {
    public:
        std::string name;
        std::vector<sh_ptr<AST> > params;
        sh_ptr<AST> body;

        FunctionDeclarationAST(int line, const std::string &name, std::vector<sh_ptr<AST> > params, sh_ptr<AST> body)
            : name(name), params(params), body(body)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "FunctionDeclaration";
        }
    };

    class ClassDeclarationAST : public AST
    {
    public:
        std::string name;
        sh_ptr<AST> superClass;
        sh_ptr<AST> body;

        ClassDeclarationAST(int line, std::string name, sh_ptr<AST> superClass, sh_ptr<AST> body)
            : name(name), superClass(superClass), body(body)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "ClassDeclaration";
        }
    };

    class NamespaceDeclarationAST : public AST
    {
    public:
        std::string name;
        sh_ptr<AST> body;

        NamespaceDeclarationAST(int line, std::string name, sh_ptr<AST> body)
            : name(name), body(body)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "NamespaceDeclaration";
        }
    };

    class ReturnStatementAST : public AST
    {
    public:
        sh_ptr<AST> argument;

        ReturnStatementAST(int line, sh_ptr<AST> argument)
            : argument(argument)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "ReturnStatement";
        }
    };

    class RequireStatementAST : public AST
    {
    public:
        std::string path;

        RequireStatementAST(int line, const std::string &path)
            : path(path)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "RequireStatement";
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
            : body(body), test(test)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "DoWhileStatement";
        }
    };

    class ForStatementAST : public AST
    {
    public:
        sh_ptr<AST> init;
        sh_ptr<AST> test;
        sh_ptr<AST> update;
        sh_ptr<AST> body;

        ForStatementAST(int line, sh_ptr<AST> init, sh_ptr<AST> test, sh_ptr<AST> update, sh_ptr<AST> body)
            : init(init), test(test), update(update), body(body)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "ForStatement";
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

    class AssignmentExpressionAST : public AST
    {
    public:
        std::string op;
        sh_ptr<AST> left;
        sh_ptr<AST> right;

        AssignmentExpressionAST(int line, const std::string &op, sh_ptr<AST> left, sh_ptr<AST> right)
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
        sh_ptr<AST> left;
        sh_ptr<AST> right;

        LogicalExpressionAST(int line, const std::string &op, sh_ptr<AST> left, sh_ptr<AST> right)
            : op(op), left(left), right(right)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "LogicalExpression";
        }
    };

    class UnaryExpressionAST : public AST
    {
    public:
        std::string op;
        sh_ptr<AST> argument;

        UnaryExpressionAST(int line, const std::string &op, sh_ptr<AST> argument)
            : op(op), argument(argument)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "UnaryExpression";
        }
    };

    class BinaryExpressionAST : public AST
    {
    public:
        std::string op;
        sh_ptr<AST> left;
        sh_ptr<AST> right;

        BinaryExpressionAST(int line, const std::string &op, sh_ptr<AST> left, sh_ptr<AST> right)
            : op(op), left(left), right(right)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "BinaryExpression";
        }
    };

    class CallExpressionAST : public AST
    {
    public:
        sh_ptr<AST> callee;
        std::vector<sh_ptr<AST> > arguments;
        bool computed;

        CallExpressionAST(int line, sh_ptr<AST> callee, std::vector<sh_ptr<AST> > arguments)
            : callee(callee), arguments(arguments)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "CallExpression";
        }
    };

    class MemberExpressionAST : public AST
    {
    public:
        sh_ptr<AST> object;
        sh_ptr<AST> property;
        std::string subtype;

        MemberExpressionAST(int line, sh_ptr<AST> object, sh_ptr<AST> property, std::string subtype)
            : object(object), property(property), subtype(subtype)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "MemberExpression";
        }
    };

    class IntLiteralAST : public AST
    {
    public:
        int value;

        IntLiteralAST(int line, std::int32_t value)
            : value(value)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "IntLiteral";
        }
    };

    class DoubleLiteralAST : public AST
    {
    public:
        double value;

        DoubleLiteralAST(int line, double value)
            : value(value)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "DoubleLiteral";
        }
    };

    class CharLiteralAST : public AST
    {
    public:
        char value;

        CharLiteralAST(int line, char value)
            : value(value)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "CharLiteral";
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

    class ListLiteralAST : public AST
    {
    public:
        std::vector<sh_ptr<AST> > elements;

        ListLiteralAST(int line, const std::vector<sh_ptr<AST> > &elements)
            : elements(elements)
        {
            this->line = line;
        }

        std::string type() const
        {
            return "ListLiteral";
        }
    };
}

#endif