#include <cctype>
#include <string>
#include <vector>

#include "token.h"
#include "lexer.h"
#include "error.h"

namespace eris {
    Lexer::Lexer(const std::string& text, const std::string& filename)
        : text(text), filename(filename),  pos(0), line(1), current(0) {
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
            else if (std::isdigit(current)) {
                tokens.push_back(generate_number());
            }
            else if (current == '_' || std::isalpha(current)) {
                tokens.push_back(generate_identifier());
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
            else if (current == '=') {
                advance();
                tokens.push_back(Token(TokenType::EQ, line));
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

        while (current != 0 && (current == '.' || std::isdigit(current))) {
            if (current == '.') {
                if (++decimal_point_count > 1) {
                    break;
                }
            }

            number_str += current;
            advance();
        }

        return Token(TokenType::NUMBER, number_str, line);
    }

    Token Lexer::generate_identifier() {
        std::string identifier(1, current);
        advance();

        while (current != 0 && (current == '_' | std::isalnum(current))) {
            identifier += current;
            advance();
        }

        if (identifier == "null") {
            return Token(TokenType::NULL_, line);
        } else if (identifier == "true") {
            return Token(TokenType::TRUE, line);
        } else if (identifier == "false") {
            return Token(TokenType::FALSE, line);
        } else if (identifier == "var") {
            return Token(TokenType::VAR, line);
        } else {
            return Token(TokenType::IDENTIFIER, identifier, line);
        }
    }
} // namespace eris