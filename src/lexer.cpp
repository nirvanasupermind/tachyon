#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "lexer.h"
#include "tokens.h"
#include "exceptions.h"

namespace eris
{

    Lexer::Lexer(const std::string &filename, const std::string &text)
        : filename(filename), text(text), line(1), pos(0), current(0)
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

        if (current == '\n')
        {
            line++;
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
                tokens.push_back(Token(line, TokenType::PLUS));
            }
            else if (current == '-')
            {
                advance();
                tokens.push_back(Token(line, TokenType::MINUS));
            }
            else if (current == '*')
            {
                advance();
                tokens.push_back(Token(line, TokenType::MULTIPLY));
            }
            else if (current == '/')
            {
                advance();
                tokens.push_back(Token(line, TokenType::DIVIDE));
            }
            else if (current == '(')
            {
                advance();
                tokens.push_back(Token(line, TokenType::LPAREN));
            }
            else if (current == ')')
            {
                advance();
                tokens.push_back(Token(line, TokenType::RPAREN));
            }
            else
            {
                std::ostringstream oss;
                oss << filename << ":" << line << ": illegal character '" << current << "'";
                throw Exception(oss.str());
            }
        }

        return tokens;
    }

    Token Lexer::generate_number()
    {
        int orig_line = line;

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

        return Token(orig_line, TokenType::NUMBER, std::stod(number_str));
    }
} // namespace eris