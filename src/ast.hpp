#ifndef AST_HPP
#define AST_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace eris
{
    /// AST - Base class for all nodes.
    class AST
    {
    public:
        virtual ~AST() {}
        virtual std::string str() = 0;
    };

    /// NumericLiteralAST - Class for numeric literals like '1.0'.
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

    /// NumericLiteralAST - Class for numeric literals like '1.0'.
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

}

#endif