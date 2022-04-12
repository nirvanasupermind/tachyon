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
        int orig_line = line;
        
        std::vector<Token> tokens;

        while (current)
        {
            if (WHITESPACE.find_first_of(current) != std::string::npos)
            {
                advance();
            }
            else if (current == '.' || isdigit(current))
            {
                tokens.push_back(generate_number());
            }
            else if (current == '_'  || isalpha(current))
            {
                tokens.push_back(generate_identifier());
            }
            else if (current == '+')
            {
                advance();
                tokens.push_back(Token(line, TokenType::PLUS, "+"));
            }
            else if (current == '-')
            {
                advance();
                tokens.push_back(Token(line, TokenType::MINUS, "-"));
            }
            else if (current == '*')
            {
                advance();
                tokens.push_back(Token(line, TokenType::MULTIPLY, "*"));
            }
            else if (current == '/')
            {
                advance();
                tokens.push_back(Token(line, TokenType::DIVIDE, "/"));
            }
            else if (current == '(')
            {
                advance();
                tokens.push_back(Token(line, TokenType::LPAREN, "("));
            }
            else if (current == ')')
            {
                advance();
                tokens.push_back(Token(line, TokenType::RPAREN, ")"));
            }
            else
            {
                std::ostringstream oss;
                oss << filename << ":" << line << ": illegal character '" << current << "'";
                throw Exception(oss.str());
            }
        }

        tokens.push_back(Token(line, TokenType::EOF_, "<EOF>"));

        return tokens;
    }

    Token Lexer::generate_number()
    {
        int orig_line = line;

        std::size_t decimal_point_count = 0;
        std::string number_str(1, current);
        advance();

        while (current && (current == '.' || isdigit(current)))
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

        if (decimal_point_count > 0)
        {
            return Token(orig_line, TokenType::DOUBLE, number_str);
        }

        return Token(orig_line, TokenType::INT, number_str);
    }

    Token Lexer::generate_identifier()
    {
        int orig_line = line;

        std::string identifier_str(1, current);
        advance();

        while (current && (current == '_' || isalnum(current)))
        {
            identifier_str += current;
            advance();
        }

        return Token(orig_line, TokenType::IDENTIFIER, identifier_str);
    }
} // namespace eris