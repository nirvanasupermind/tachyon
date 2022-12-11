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
            else if (current == '"') {
                tokens.push_back(generate_string());
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
                
                if(current == '/') {
                    // Single-line comment
                    while(current && current != '\n') {
                        advance();
                    }
                } else {
                    tokens.push_back(Token(TokenType::DIV, line));
                }
            }
            else if (current == '%') {
                advance();
                tokens.push_back(Token(TokenType::MOD, line));
            }
            else if (current == '<') {
                advance();
                
                if(current == '=') {
                    advance();
                    tokens.push_back(Token(TokenType::LE, line));
                } else {
                    tokens.push_back(Token(TokenType::LT, line));
                }
            }
            else if (current == '>') {
                advance();
                
                if(current == '=') {
                    advance();
                    tokens.push_back(Token(TokenType::GE, line));
                } else {
                    tokens.push_back(Token(TokenType::GT, line));
                }
            }
            else if (current == '=') {
                int token_line = line;

                advance();
                
                if(current == '=') {
                    advance();
                    tokens.push_back(Token(TokenType::EE, line));
                } else {
                    tokens.push_back(Token(TokenType::EQ, line));
                }
            }

            else if (current == '!') {
                advance();

                if(current == '=') {
                    advance();
                    tokens.push_back(Token(TokenType::NE, line));
                }
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

        while (current && (current == '.' || std::isdigit(current))) {
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

    Token Lexer::generate_string() {
        std::size_t decimal_point_count = 0;
        std::string str;
        advance();

        while (current && current != '"') {
            str += current;
            advance();
        }

        advance();
        return Token(TokenType::STRING, str, line);
    }

    Token Lexer::generate_identifier() {
        std::string identifier(1, current);
        advance();

        while (current && (current == '_' | std::isalnum(current))) {
            identifier += current;
            advance();
        }

        if (KEYWORDS.count(identifier)) {
            return Token(KEYWORDS.at(identifier), line);
        } else {
            return Token(TokenType::IDENTIFIER, identifier, line);
        }
    }
} // namespace eris