#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

namespace eris {
    class Lexer {
    private:
        std::string text{};
        std::string filename{};
        char current;
        int pos;
        int line;
        void advance();
        Token generate_number();
        Token generate_identifier();
    public:
        Lexer(const std::string& text, const std::string& filename);
        std::vector<Token> generate_tokens();
    };
} // namespace eris


#endif // LEXER_H