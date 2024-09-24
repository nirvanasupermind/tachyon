#include <string>
#include <vector>
#include "token.h"
#include "lexer.h"

namespace tachyon {
    Lexer::Lexer(const std::string& fn, const std::string& text) {
        this->fn = fn;
        this->text = text;
        this->ln = 1;
        this->pos = -1;
        this->current_char = '\0';
        advance();
    }

    void Lexer::advance() {
        pos++;
        current_char = pos < text.size() ? text.at(pos) : '\0';
        if(current_char == '\n') {
            ln++;    
        }
    }

    std::vector<Token> Lexer::make_tokens() {
        std::vector<Token> tokens;
        while(current_char != '\0') {
            if(current_char == ' ' || current_char == '\t') {
                advance();
            }  else if(isdigit(current_char)) {
                tokens.push_back(make_number());
            } else if(current_char == '+') {
                tokens.push_back(Token(ln, TokenType::PLUS, "+"));
                advance();
            } else if(current_char == '-') {
                tokens.push_back(Token(ln, TokenType::MINUS, "-"));
                advance();
            } else if(current_char == '*') {
                tokens.push_back(Token(ln, TokenType::MUL, "*"));
                advance();
            }  else if(current_char == '/') {
                tokens.push_back(Token(ln, TokenType::DIV, "/"));
                advance();
            }  else if(current_char == '(') {
                tokens.push_back(Token(ln, TokenType::LPAREN, "("));
                advance();
            }  else if(current_char == ')') {
                tokens.push_back(Token(ln, TokenType::RPAREN, " )"));
                advance();
            } else {
                throw std::runtime_error(fn + ":" + std::to_string(ln) + ": " + "illegal character: '" + current_char + "'");
            }
        }
        return tokens;
    }

    Token Lexer::make_number() {
        std::string num_str = "";
        int dot_count = 0;
        while(current_char != '\0' && (isdigit(current_char) || current_char == '.')) {
            if(current_char == '.') {
                dot_count += 1; 
                num_str += '.';
            } else {
                num_str += current_char;
            }
            advance();
        }

        if(dot_count == 0) {
            return Token(ln, TokenType::INT, num_str);
        } else {
            return Token(ln, TokenType::FLOAT, num_str);
        }
    }
};