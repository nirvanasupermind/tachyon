#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "token.h"
#include "node.h"

namespace tachyon {
    class Parser {
    public:
        std::string fn;
        std::vector<Token> tokens;
        int tok_idx;
        Token current_tok;
        Parser(const std::string& fn, const std::vector<Token>& tokens);
        void advance();
        void raise_error();
        std::shared_ptr<Node> parse();
        std::shared_ptr<Node> factor();
        std::shared_ptr<Node> term();
        std::shared_ptr<Node> expr();
        std::shared_ptr<Node> bin_op(const std::function<std::shared_ptr<Node>()>& func, const std::vector<TokenType>& ops);
    };
};

#endif