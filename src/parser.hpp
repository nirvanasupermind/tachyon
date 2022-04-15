#ifndef PARSER_HPP
#define PARSER_HPP

#include "tokens.hpp"
#include "tokenizer.hpp"
#include "ast.hpp"
#include "aliases.hpp"

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
        std::vector<sh_ptr<AST> > parse(const std::string &string)
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
        std::vector<sh_ptr<AST> > Program()
        {
            return this->StatementList();
        }

        /**
         * StatementList
         *  : Statement
         *  | StatementList Statement
         *  ;
         */
        std::vector<sh_ptr<AST> > StatementList(const std::string &stopLookahead = "EOF")
        {
            std::vector<sh_ptr<AST> > statementList { Statement() };

            while (this->lookahead.type != stopLookahead) 
            {
                statementList.push_back(this->Statement());
            }
            
            return statementList;
        }

        /**
         * Statement
         *  : ExpressionStatement
         *  | BlockStatement
         *  ;
         */
        sh_ptr<AST> Statement() 
        {
            if (this->lookahead.type == "{") 
            {
                return BlockStatement();
            }

            return this->ExpressionStatement();
        }
        
        /**
         * BlockStatement
         *  : '{' OptStatementList '}' 
         *  ;
         */
        
        sh_ptr<AST> BlockStatement() 
        {
            this->eat("{");

            // OptStatementList
            std::vector<sh_ptr<AST> > body; 

            if(this->lookahead.type != "}")
                body = this->StatementList("}");

            this->eat("}");
            
            return sh_ptr<AST>(new BlockStatementAST(body));
        }

        /**
         * ExpressionStatement
         *  : Expression ';'
         *  ;
         */
        sh_ptr<AST> ExpressionStatement() 
        {
            sh_ptr<AST> expression = this->Expression();

            eat(";");

            return sh_ptr<ExpressionStatementAST>(new ExpressionStatementAST(expression));
        }

        /**
         * Expression
         *  : Literal
         *  ;
         */
        sh_ptr<AST> Expression() 
        {
            return Literal();                
        }

        /**
         * Literal 
         *  : NumericLiteral
         *  | StringLiteral
         *  ;
         */
        sh_ptr<AST> Literal()
        {
            if (lookahead.type == "NUMBER")
                return NumericLiteral();
            
            if (lookahead.type == "STRING")
                return StringLiteral();

            error("unexpected literal production");
            return sh_ptr<AST>();
        }

        /**
         * NumericLiteral
         *  :
         *  NUMBER
         *  ;
         */
        sh_ptr<AST> NumericLiteral()
        {
            Token token = eat("NUMBER");

            return sh_ptr<NumericLiteralAST>(new NumericLiteralAST(std::stod(token.lexeme)));
        }
        
        /**
         * StringLiteral 
         *  :
         *  STRING
         *  ;
         */
        sh_ptr<AST> StringLiteral()
        {
            Token token = eat("STRING");

            return sh_ptr<StringLiteralAST>(new StringLiteralAST(token.lexeme.substr(1, token.lexeme.size() - 2)));
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