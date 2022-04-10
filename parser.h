#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>
#include <vector>

#include "tokens.h"
#include "nodes.h"

namespace eris {

class Parser {
private:
    std::string filename;
    std::vector<Token> tokens;
    std::size_t pos;
    Token current;

    void raise_error() const;
    void advance();

    std::unique_ptr<Node> expr();
    std::unique_ptr<Node> term();
    std::unique_ptr<Node> factor();
public:
    Parser(const std::string &filename, const std::vector<Token>& tokens);

    std::unique_ptr<Node> parse();
};

} // namespace eris


#endif // PARSER_H