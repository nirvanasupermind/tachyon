#include <iostream>
#include <string>
#include <vector>
#include "token.h"
#include "lexer.h"

namespace eris {
    Lexer::Lexer(const std::string& text, const std::string& filename)
        : text(text), current('\0'), pos(0), line(1), filename(filename) {
        advance();
    }

    void Lexer::advance() {
        if (pos < (int)text.length()) {
            current = text.at(pos);
            pos++;
            if(current == '\n') {
                line++;
            }
        }
        else {
            current = '\0';
        }
    }

    std::vector<Token> Lexer::generate_tokens() {
        std::vector<Token> tokens;
        while (current != '\0') {
            if(current == ' ') {
                advance();
            } else if (isdigit(current)) {
                tokens.push_back(generate_number());
                // advance();
            }
            else if (current == '+') {
                tokens.push_back(Token(TokenType::PLUS, "+", line));
                advance();
            }
            else if (current == '-') {
                tokens.push_back(Token(TokenType::MINUS, "-", line));
                advance();
            }
            else if (current == '*') {
                tokens.push_back(Token(TokenType::MUL, "*", line));
                advance();
            }
            else if (current == '/') {
                tokens.push_back(Token(TokenType::DIV, "/", line));
                advance();
            }
            else if (current == '(') {
                tokens.push_back(Token(TokenType::LPAREN, "(", line));
                advance();
            }
            else if (current == ')') {
                tokens.push_back(Token(TokenType::LPAREN, ")", line));
                advance();
            }
            else if (current == ';') {
                tokens.push_back(Token(TokenType::SEMICOLON, ";", line));
                advance();
            } else {
                throw std::string(filename + ":" + std::to_string(line) + ": illegal character '" + current + "'");
            }
        }

        tokens.push_back(Token(TokenType::EOF_, "<eof>", line));

        return tokens;
    }

    Token Lexer::generate_number() {
        std::string number_str;
        int decimal_point_count = 0;
        while (current != '\0' && (current == '.' || isdigit(current))) {
            if(current == '.') {
                decimal_point_count++;
                if(decimal_point_count >= 2) {
                    break;
                }
            }

            number_str = number_str + current;
            advance();
        }
        return Token(TokenType::NUMBER, number_str, line);
    }
} // namespace eris