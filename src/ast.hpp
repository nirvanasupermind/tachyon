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

    /// NumericLiteralAST - Class for numeric literals like "1.0".
    class NumericLiteralAST : public AST
    {
        double Val;

    public:
        NumericLiteralAST(double Val) : Val(Val) {}

        std::string str()
        {
            return std::to_string(Val);
        }
    };
}

#endif