#ifndef PARSER_HPP
#define PARSER_HPP

// #include <functional>
#include <string>
#include <memory>
#include <vector>

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

        /**
         * Generic binary expression.
         */
        sh_ptr<AST> BinaryExpression(sh_ptr<AST> (Parser::*builder)(), const std::string &operatorToken)
        {
            int line = this->tokenizer.line;

            sh_ptr<AST> left = (this->*builder)();

            while (this->lookahead.type == operatorToken)
            {
                // Operator: *, /

                std::string op = this->lookahead.lexeme;

                this->eat(operatorToken);

                sh_ptr<AST> right = (this->*builder)();

                left = sh_ptr<BinaryExpressionAST>(new BinaryExpressionAST(line, op, left, right));
            }

            return left;
        }

    public:
        /**
         * Initializes the parser.
         */
        Parser() : string(""), tokenizer(Tokenizer()) {}

        void error(const std::string &message) const
        {
            throw std::string(std::to_string(this->tokenizer.line) + ": syntax error: " + message);
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
            std::vector<sh_ptr<AST> > statementList{this->Statement()};

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
            if (this->lookahead.type == ";")
            {
                return this->EmptyStatement();
            }

            if (this->lookahead.type == "{")
            {
                return this->BlockStatement();
            }

            return this->ExpressionStatement();
        }

        /**
         * EmptyStatement
         *  : ';'
         *  ;
         */

        sh_ptr<AST> EmptyStatement()
        {
            int line = this->tokenizer.line;

            this->eat(";");

            return sh_ptr<AST>(new EmptyStatementAST(line));
        }

        /**
         * BlockStatement
         *  : '{' OptStatementList '}'
         *  ;
         */

        sh_ptr<AST> BlockStatement()
        {
            int line = this->tokenizer.line;

            this->eat("{");

            // OptStatementList
            std::vector<sh_ptr<AST> > body;

            if (this->lookahead.type != "}")
                body = this->StatementList("}");

            this->eat("}");

            return sh_ptr<BlockStatementAST>(new BlockStatementAST(line, body));
        }

        /**
         * ExpressionStatement
         *  : Expression ';'
         *  ;
         */
        sh_ptr<AST> ExpressionStatement()
        {
            int line = this->tokenizer.line;

            sh_ptr<AST> expression = this->Expression();

            this->eat(";");

            return sh_ptr<ExpressionStatementAST>(new ExpressionStatementAST(line, expression));
        }

        /**
         * Expression
         *  : AdditiveExpression
         *  ;
         */
        sh_ptr<AST> Expression()
        {
            return AdditiveExpression();
        }

        /**
         * AdditiveExpression
         *  : Literal
         *  | AdditiveExpression ADDITIVE_OPERATOR Literal
         *  ;
         */
        sh_ptr<AST> AdditiveExpression()
        {
            return this->BinaryExpression(&Parser::MultiplicativeExpression, "ADDITIVE_OPERATOR");
        }

        /**
         * MultiplicativeExpression
         *  : Literal
         *  | MultiplicativeExpression MULTIPLICATIVE_OPERATOR Literal
         *  ;
         */
        sh_ptr<AST> MultiplicativeExpression()
        {
            return this->BinaryExpression(&Parser::PrimaryExpression, "ADDITIVE_OPERATOR");
        }

        /**
         * PrimaryExpression
         *  : Literal
         *  | ParenthesizedExpression
         *  ;
         */
        sh_ptr<AST> PrimaryExpression()
        {
            if (this->lookahead.type == "(")
            {
                return this->ParenthesizedExpression();
            }

            return this->Literal();
        }

        sh_ptr<AST> ParenthesizedExpression()
        {
            eat("(");

            sh_ptr<AST> expression = this->Expression();

            eat(")");

            return expression;
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
                return this->NumericLiteral();

            if (lookahead.type == "STRING")
                return this->StringLiteral();

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
            int line = this->tokenizer.line;

            Token token = this->eat("NUMBER");

            return sh_ptr<NumericLiteralAST>(new NumericLiteralAST(line, std::stod(token.lexeme)));
        }

        /**
         * StringLiteral
         *  :
         *  STRING
         *  ;
         */
        sh_ptr<AST> StringLiteral()
        {
            int line = this->tokenizer.line;

            Token token = this->eat("STRING");

            return sh_ptr<StringLiteralAST>(new StringLiteralAST(line, token.lexeme.substr(1, token.lexeme.size() - 2)));
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