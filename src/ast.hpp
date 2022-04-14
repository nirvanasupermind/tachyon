#ifndef AST_HPP
#define AST_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace eris
{
    class AST {
    public:
        virtual ~AST() {}
        virtual std::string str() = 0;
    };

    class NumericLiteralAST : public AST
    {
    public:
        double value;

        NumericLiteralAST(double value) : value(value) {}

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

        std::string str()
        {
            std::string result = "StatementList(";
            for(std::size_t i = 0; i < statementList.size(); i++) 
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

        std::string str()
        {
            return "ExpressionStatement(" + expression->str() + ")";
        }
    };
}

#endif