#ifndef AST_HPP
#define AST_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace eris
{
    enum class ASTType
    {
        NumericLiteral,
        StringLiteral,
        StatementList,
        ExpressionStatement,
    };

    class AST
    {
    public:
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

    class StatementListAST : public AST
    {
    public:
        std::vector<std::shared_ptr<AST> > statementList;

        StatementListAST(const std::vector<std::shared_ptr<AST> > &statementList)
            : statementList(statementList)
        {
        }

        ASTType type() const
        {
            return ASTType::StatementList;
        }

        std::string str()
        {
            std::string result = "StatementList(";
            for (std::size_t i = 0; i < statementList.size(); i++)
            {
                result += statementList.at(i)->str() + ",";
            }

            result.pop_back();

            result += ")";

            return result;
        }
    };

    class ExpressionStatementAST : public AST
    {
    public:
        std::shared_ptr<AST> expression;

        ExpressionStatementAST(std::shared_ptr<AST> expression)
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