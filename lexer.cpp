#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "lexer.h"
#include "tokens.h"

namespace volcano
{

    Lexer::Lexer(const std::string &text)
        : text(text), pos(0), current(0)
    {
        advance();
    }

    void Lexer::advance()
    {
        if (pos < text.length())
        {
            current = text.at(pos++);
        }
        else
        {
            current = 0; // 0 means nothing, therefore in the while loop, it will break if current == 0.
        }
    }

    std::vector<Token> Lexer::generate_tokens()
    {
        std::vector<Token> tokens;

        while (current)
        {
            if (WHITESPACE.find_first_of(current) != std::string::npos)
            {
                advance();
            }
            else if (current == '.' || DIGITS.find_first_of(current) != std::string::npos)
            {
                tokens.push_back(generate_number());
            }
            else if (current == '+')
            {
                advance();
                tokens.push_back(Token(TokenType::PLUS));
            }
            else if (current == '-')
            {
                advance();
                tokens.push_back(Token(TokenType::MINUS));
            }
            else if (current == '*')
            {
                advance();
                tokens.push_back(Token(TokenType::MULTIPLY));
            }
            else if (current == '/')
            {
                advance();
                tokens.push_back(Token(TokenType::DIVIDE));
            }
            else if (current == '(')
            {
                advance();
                tokens.push_back(Token(TokenType::LPAREN));
            }
            else if (current == ')')
            {
                advance();
                tokens.push_back(Token(TokenType::RPAREN));
            }
            else
            {
                std::ostringstream oss;
                oss << "Illegal character '" << current << "'";
                throw oss.str();
            }
        }

        return tokens;
    }

    Token Lexer::generate_number()
    {
        std::size_t decimal_point_count = 0;
        std::string number_str(1, current);
        advance();

        while (current && (current == '.' || DIGITS.find_first_of(current) != std::string::npos))
        {
            if (current == '.')
            {
                if (++decimal_point_count > 1)
                {
                    break;
                }
            }

            number_str += current;
            advance();
        }

        if (number_str.at(0) == '.')
        {
            number_str = '0' + number_str;
        }

        if (number_str.at(number_str.length() - 1) == '.')
        {
            number_str += '0';
        }

        return Token(TokenType::NUMBER, std::stod(number_str));
    }

} // namespace volcano