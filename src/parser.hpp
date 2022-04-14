#ifndef PARSER_HPP
#define PARSER_HPP

#include "tokenizer.hpp"
#include "ast.hpp"

namespace eris
{
    /**
     * Eris parser: recursive descent implementation.
     */

    class Parser
    {
    private:
        std::string string;
        Tokenizer tokenizer;
        Token lookahead;

    public:
        /**
         * Initializes the parser.
         */
        Parser() : string(""), tokenizer(Tokenizer()) {}

        void error(const std::string &message) const
        {
            throw std::string(std::to_string(tokenizer.line) + ": " + message);
        }

        /**
         * Parses a string.
         */
        std::shared_ptr<AST> parse(const std::string &string)
        {
            this->string = string;
            this->tokenizer.init(string);

            // Prime the tokenizer to obtain the first
            // token which is our lookahead. The lookahead is
            // used for predictive parsing.

            this->lookahead = this->tokenizer.getNextToken();

            return Program();
        }

        /**
         * Main entry point.
         *
         * Program
         *  :
         *  StatementList
         *  ;
         */
        std::shared_ptr<AST> Program()
        {
            return this->StatementList();
        }

        /**
         * StatementList
         *  : Statement
         *  | StatementList Statement
         *  ;
         */
        std::shared_ptr<AST> StatementList()
        {
            std::vector<std::shared_ptr<AST> > statementList { Statement() };

            while (this->lookahead.type != "EOF") 
            {
                statementList.push_back(this->Statement());
            }
            
            return std::shared_ptr<StatementListAST>(new StatementListAST(statementList));
        }

        /**
         * Statement
         *  : ExpressionStatement
         *  ;
         */
        std::shared_ptr<AST> Statement() 
        {
            return this->ExpressionStatement();
        }

        /**
         * ExpressionStatement
         *  : Expression ';'
         *  ;
         */
        std::shared_ptr<AST> ExpressionStatement() 
        {
            std::shared_ptr<AST> expression = this->Expression();

            eat(";");

            return std::shared_ptr<ExpressionStatementAST>(new ExpressionStatementAST(expression));
        }

        /**
         * Expression
         *  : Literal
         *  ;
         */
        std::shared_ptr<AST> Expression() 
        {
            return Literal();                
        }

        /**
         * Literal 
         *  : NumericLiteral
         *  | StringLiteral
         *  ;
         */
        std::shared_ptr<AST> Literal()
        {
            if (lookahead.type == "NUMBER")
                return NumericLiteral();
            
            if (lookahead.type == "STRING")
                return StringLiteral();

            error("unexpected literal production");
            return std::shared_ptr<AST>();
        }

        /**
         * NumericLiteral
         *  :
         *  NUMBER
         *  ;
         */
        std::shared_ptr<AST> NumericLiteral()
        {
            Token token = eat("NUMBER");

            return std::shared_ptr<NumericLiteralAST>(new NumericLiteralAST(std::stod(token.lexeme)));
        }
        
        /**
         * StringLiteral 
         *  :
         *  STRING
         *  ;
         */
        std::shared_ptr<AST> StringLiteral()
        {
            Token token = eat("STRING");

            return std::shared_ptr<StringLiteralAST>(new StringLiteralAST(token.lexeme));
        }

        /**
         * Expects a token of a given type.
         */

        Token eat(const std::string &tokenType)
        {
            Token token = this->lookahead;

            if (token.type == "EOF")
            {
                error("unexpected end of input, expected: \"" + tokenType + "\"");
                return Token();
            }

            if (token.type == "EOF")
            {
                error("unexpected end of input, expected: \"" + tokenType + "\"");
                return Token();
            }

            if (token.type != tokenType)
            {
                error("unexpected token: \"" + token.lexeme + "\", expected: " + tokenType);
                return Token();
            }

            // Advance to next token.
            this->lookahead = this->tokenizer.getNextToken();

            return token;
        }
    };
}

#endif