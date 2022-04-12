#include <string>
#include <memory>
#include <vector>
#include <cstdint>

#include "parser.h"

namespace eris
{

    Parser::Parser(const std::string &filename, const std::vector<Token> &tokens)
        : filename(filename), tokens(tokens),
          pos(0),
          current(Token(0, TokenType::EOF_, "<EOF>"))
    {
        advance();
    }

    void Parser::raise_error() const
    {
        std::ostringstream oss;
        oss << filename << ":" << current.line << ": syntax error near '" << current.lexeme + "'";
        throw Exception(oss.str());
    }

    void Parser::advance()
    {
        current = tokens.at(pos++);
    }

    std::shared_ptr<Node> Parser::parse()
    {
        if (!current)
        {
            return std::shared_ptr<Node>();
        }

        std::shared_ptr<Node> result = expr();

        if (current)
        {
            raise_error();
        }

        return result;
    }

    std::shared_ptr<Node> Parser::expr()
    {
        std::shared_ptr<Node> result = term();

        while (current && (current.type == TokenType::PLUS || current.type == TokenType::MINUS))
        {
            if (current.type == TokenType::PLUS)
            {
                advance();
                result = std::shared_ptr<AddNode>(new AddNode(result->line, std::move(result), term()));
            }
            else if (current.type == TokenType::MINUS)
            {
                advance();
                result = std::shared_ptr<SubtractNode>(new SubtractNode(result->line, std::move(result), term())); // Missin' std::make_shared :(
            }
        }

        return result;
    }

    std::shared_ptr<Node> Parser::term()
    {
        std::shared_ptr<Node> result = factor();

        while (current && (current.type == TokenType::MULTIPLY || current.type == TokenType::DIVIDE))
        {
            if (current.type == TokenType::MULTIPLY)
            {
                advance();
                result = std::shared_ptr<MultiplyNode>(new MultiplyNode(result->line, std::move(result), factor()));
            }
            else if (current.type == TokenType::DIVIDE)
            {
                advance();
                result = std::shared_ptr<DivideNode>(new DivideNode(result->line, std::move(result), factor()));
            }
        }

        return result;
    }

    std::shared_ptr<Node> Parser::factor()
    {
        Token token = current;

        if (token.type == TokenType::LPAREN)
        {
            advance();
            std::shared_ptr<Node> result = expr();

            if (current.type != TokenType::RPAREN)
            {
                raise_error();
            }

            advance();
            return result;
        }
        else if (token.type == TokenType::INT)
        {
            advance();
            return std::shared_ptr<IntNode>(new IntNode(token.line, (std::int32_t)std::stol(token.lexeme)));
        }
        else if (token.type == TokenType::DOUBLE)
        {
            advance();
            return std::shared_ptr<DoubleNode>(new DoubleNode(token.line, std::stod(token.lexeme)));
        }
        else if (token.type == TokenType::IDENTIFIER)
        {
            advance();
            return std::shared_ptr<IdentifierNode>(new IdentifierNode(token.line, token.lexeme));
        }
        else if (token.type == TokenType::COLON)
        {
            advance();

            if (current.type != TokenType::IDENTIFIER) {
                raise_error();
            }

            std::string atom = current.lexeme;

            advance();

            return std::shared_ptr<AtomNode>(new AtomNode(token.line, atom));
        }
        else if (token.type == TokenType::PLUS)
        {
            advance();
            return std::shared_ptr<PlusNode>(new PlusNode(token.line, factor()));
        }
        else if (token.type == TokenType::MINUS)
        {
            advance();
            return std::shared_ptr<MinusNode>(new MinusNode(token.line, factor()));
        }

        raise_error();
        return std::shared_ptr<Node>();
    }
} // namespace eris
