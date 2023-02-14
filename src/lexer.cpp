#include <string>
#include <vector>
#include <map>
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
            if (current == '\n') {
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
            if (isspace(current)) {
                advance();
            }
            else if (isdigit(current)) {
                tokens.push_back(generate_number());
                // advance();
            }
            else if (current == '_' || current == '$' || isalpha(current)) {
                tokens.push_back(generate_identifier());
                // advance();
            }
            else if (current == '\'') {
                advance();
                char val = current;
                advance();
                if(current == '\'') {
                    tokens.push_back(Token(TokenType::CHAR, std::string("'") + val + std::string("'"), line));
                    advance();
                }
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
            else if (current == '%') {
                tokens.push_back(Token(TokenType::MOD, "%", line));
                advance();
            }
            else if (current == '=') {
                int ln = line;
                advance();
                if (current == '=') {
                    tokens.push_back(Token(TokenType::EE, "==", ln));
                    advance();
                } else {
                    tokens.push_back(Token(TokenType::EQ, "=", ln));
                }
            }
            else if (current == '!') {
                int ln = line;
                advance();
                if (current == '=') {
                    tokens.push_back(Token(TokenType::NE, "!=", ln));
                    advance();
                }
            }
            else if (current == '<') {
                int ln = line;
                advance();
                if (current == '<') {
                    tokens.push_back(Token(TokenType::SR, "<<", ln));
                    advance();
                } else if(current == '=') {
                    tokens.push_back(Token(TokenType::LE, "<=", ln));
                    advance();
                } else {
                    tokens.push_back(Token(TokenType::LT, "<", ln));
                }
            }
            else if (current == '>') {
                int ln = line;
                advance();
                if (current == '>') {
                    tokens.push_back(Token(TokenType::SL, ">>", ln));
                    advance();
                } else if(current == '=') {
                    tokens.push_back(Token(TokenType::GE, ">=", ln));
                    advance();
                } else {
                    tokens.push_back(Token(TokenType::GT, ">", ln));
                }
            }
            else if (current == '&') {
                int ln = line;
                advance();
                if (current == '&') {
                    tokens.push_back(Token(TokenType::AND, "&&", ln));
                    advance();
                } else {
                    tokens.push_back(Token(TokenType::BITAND, "&", ln));
                }
            }
            else if (current == '|') {
                int ln = line;
                advance();
                if (current == '|') {
                    tokens.push_back(Token(TokenType::OR, "||", ln));
                    advance();
                } else {
                    tokens.push_back(Token(TokenType::BITOR, "|", ln));
                }
            }
            else if (current == '^') {
                int ln = line;
                advance();
                if (current == '^') {
                    tokens.push_back(Token(TokenType::XOR, "^^", ln));
                    advance();
                } else {
                    tokens.push_back(Token(TokenType::BITXOR, "^", ln));
                }
            }
            else if (current == '(') {
                tokens.push_back(Token(TokenType::LPAREN, "(", line));
                advance();
            }
            else if (current == ')') {
                tokens.push_back(Token(TokenType::RPAREN, ")", line));
                advance();
            }
            else if (current == '{') {
                tokens.push_back(Token(TokenType::LCURLY, "{", line));
                advance();
            }
            else if (current == '}') {
                tokens.push_back(Token(TokenType::RCURLY, "}", line));
                advance();
            }
            else if (current == ',') {
                tokens.push_back(Token(TokenType::COMMA, ",", line));
                advance();
            }
            else if (current == ';') {
                tokens.push_back(Token(TokenType::SEMICOLON, ";", line));
                advance();
            }
            else {
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
            if (current == '.') {
                decimal_point_count++;
                if (decimal_point_count >= 2) {
                    break;
                }
            }

            number_str = number_str + current;
            advance();
        }
        return Token(TokenType::NUMBER, number_str, line);
    }

    Token Lexer::generate_identifier() {
        std::string identifier_str;
        while (current != '\0' && (current == '_' || current == '$' || isalpha(current))) {
            identifier_str = identifier_str + current;
            advance();
        }
        if (KEYWORDS.count(identifier_str)) {
            return Token(KEYWORDS.at(identifier_str), identifier_str, line);
        }
        return Token(TokenType::IDENTIFIER, identifier_str, line);
    }
} // namespace eris