#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <functional>

#include "token.h"
#include "node.h"

namespace eris {

    class Parser {
    private:
        std::string filename{};
        std::vector<Token> tokens;
        size_t pos;
        Token current;

        void advance();
        Token eat(TokenType type);
        Node basic_expr();
        Node unary_expr();
        Node binary_expr(const std::function<Node()>& operand, TokenType op);
        Node mul_expr();
        Node div_expr();
        Node mod_expr();
        Node add_expr();
        Node sub_expr();
        Node expr();
        Node expr_statement();
        Node program();
    public:
        Parser(const std::string& filename, const std::vector<Token>& tokens);
        Node parse();
    };

} // namespace eris


#endif // PARSER_H