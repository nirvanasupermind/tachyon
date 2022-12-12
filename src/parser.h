#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "token.h"
#include "node.h"

namespace eris {
    class Parser {
    private:
        std::vector<Token> tokens;
        std::size_t pos;
        Token current;
        std::string filename{};

        void advance();
        Token eat(TokenType type);
        std::shared_ptr<Node> basic_expr();
        std::shared_ptr<Node> unary_expr();
        std::shared_ptr<Node> binary_expr(const std::function<std::shared_ptr<Node>()>& operand, const std::vector<TokenType>& op_types);
        std::shared_ptr<Node> multiplicative_expr();
        std::shared_ptr<Node> additive_expr();
        std::shared_ptr<Node> comparison_expr();
        std::shared_ptr<Node> equality_expr();
        std::shared_ptr<Node> assignment_expr();
        std::shared_ptr<Node> expr();
        std::shared_ptr<Node> expr_stmt();
        std::shared_ptr<Node> var_decl_stmt();
        std::shared_ptr<Node> stmt();
        std::shared_ptr<Node> program();
    public:
        Parser(const std::vector<Token>& tokens, const std::string& filename);
        std::shared_ptr<Node> parse();
    };

} // namespace eris


#endif // PARSER_H