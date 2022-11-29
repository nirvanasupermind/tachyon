#include <string>
#include <vector>

#include "token.h"
#include "lexer.h"
#include "error.h"

namespace eris {
    Lexer::Lexer(const std::string &filename, const std::string& text)
        : filename(filename), text(text), pos(0), line(1), current(0) {
        advance();
    }

    void Lexer::advance() {
        if (pos < text.length()) {
            current = text.at(pos++);
            if (current == '\n') {
                line++;
            }
        }
        else {
            current = 0;
        }
    }

    std::vector<Token> Lexer::generate_tokens() {
        std::vector<Token> tokens;

        while (current) {
            if (WHITESPACE.find_first_of(current) != std::string::npos) {
                advance();
            }
            else if (current == '.' || DIGITS.find_first_of(current) != std::string::npos) {
                tokens.push_back(generate_number());
            }
            else if (current == '+') {
                advance();
                tokens.push_back(Token(TokenType::PLUS, line));
            }
            else if (current == '-') {
                advance();
                tokens.push_back(Token(TokenType::MINUS, line));
            }
            else if (current == '*') {
                advance();
                tokens.push_back(Token(TokenType::MUL, line));
            }
            else if (current == '/') {
                advance();
                tokens.push_back(Token(TokenType::DIV, line));
            }
            else if (current == '%') {
                advance();
                tokens.push_back(Token(TokenType::MOD, line));
            }
            else if (current == '(') {
                advance();
                tokens.push_back(Token(TokenType::LPAREN, line));
            }
            else if (current == ')') {
                advance();
                tokens.push_back(Token(TokenType::RPAREN, line));
            }
            else if (current == ';') {
                advance();
                tokens.push_back(Token(TokenType::SEMICOLON, line));
            }
            else {
                raise_error(filename, line, "illegal character '" + std::string(1, current) + "'");
            }
        }
        
        tokens.push_back(Token(TokenType::EOF_, line));

        return tokens;
    }

    Token Lexer::generate_number() {
        std::size_t decimal_point_count = 0;
        std::string number_str(1, current);
        advance();

        while (current != 0 && (current == '.' || DIGITS.find_first_of(current) != std::string::npos)) {
            if (current == '.') {
                if (++decimal_point_count > 1) {
                    break;
                }
            }

            number_str += current;
            advance();
        }

        return Token(TokenType::NUMBER, line, number_str);
    }

} // namespace eris