#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <functional>

#include "token.h"
#include "node.h"

namespace eris {

    class Parser {
    private:
        std::vector<Token> tokens{};
        int pos;
        Token current;
        std::string filename{};
        void raise_error() const;
        Token eat(TokenType type);
        void advance();
        std::shared_ptr<Node> stmt_list();
        std::shared_ptr<Node> expr_stmt();
        std::shared_ptr<Node> expr();
        std::shared_ptr<Node> binary_expr(const std::function<std::shared_ptr<Node>()>& operand, const std::set<TokenType>& op_types);
        std::shared_ptr<Node> additive_expr();
        std::shared_ptr<Node> multiplicative_expr();
        std::shared_ptr<Node> unary_expr();
        std::shared_ptr<Node> primary_expr();
    public:
        Parser(const std::vector<Token>& tokens, const std::string& filename);
        std::shared_ptr<Node> parse();
    };

} // namespace eris


#endif // PARSER_H