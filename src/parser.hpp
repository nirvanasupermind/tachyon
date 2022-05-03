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
                std::string op = this->lookahead.value;

                this->eat(operatorToken);
                
                sh_ptr<AST> right = (this->*builder)();

                left = sh_ptr<BinaryExpressionAST>(new BinaryExpressionAST(line, op, left, right));
            }

            return left;
        }

        /**
         * Generic binary expression.
         */
        sh_ptr<AST> LogicalExpression(sh_ptr<AST> (Parser::*builder)(), const std::string &operatorToken)
        {
            int line = this->tokenizer.line;

            sh_ptr<AST> left = (this->*builder)();

            while (this->lookahead.type == operatorToken)
            {
                std::string op = this->lookahead.value;

                this->eat(operatorToken);
                
                sh_ptr<AST> right = (this->*builder)();
                
                left = sh_ptr<LogicalExpressionAST>(new LogicalExpressionAST(line, op, left, right));
            }

            return left;
        }

        sh_ptr<AST> checkValidAssignmentTarget(sh_ptr<AST> node)
        {
            if (node->type() == "Identifier")
            {
                return node;
            }

            error("invalid left-hand side in assignment expression");
            return sh_ptr<AST>();
        }

        /**
         * Whether the token type is an assignment operator.
         */
        bool isAssignmentOperator(const std::string &tokenType)
        {
            return tokenType == "SIMPLE_ASSIGN" || tokenType == "COMPLEX_ASSIGN";
        }

        /**
         * Whether the token is a literal.
         */
        bool isLiteral(const std::string &tokenType)
        {
            return tokenType == "NUMBER" || tokenType == "HEX" || tokenType == "STRING" || tokenType == "true" || tokenType == "false" || tokenType == "null";
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
         *  | EmptyStatement
         *  | BlockStatement
         *  | VariableStatement
         *  | IterationStatement
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

            if (this->lookahead.type == "if")
            {
                return this->IfStatement();
            }

            if (this->lookahead.type == "let")
            {
                return this->VariableStatement();
            }

            if (this->lookahead.type == "while" || this->lookahead.type == "do")
            {
                return this->IterationStatement();
            }

            return this->ExpressionStatement();
        }

        /**
         * IterationStatement
         *  : WhileStatement
         *  | DoWhileStatement
         *  ;
         * 
         */
        sh_ptr<AST> IterationStatement()
        {
            if(this->lookahead.type == "while")
            {
                return this->WhileStatement();
            }

            if(this->lookahead.type == "do")
            {
                return this->DoWhileStatement();
            }   

            return sh_ptr<AST>();
        }

        /**
         * WhileStatement
         *  : 'while' '(' Expression ')' Statement
         *  ;
         */
        sh_ptr<AST> WhileStatement()
        {
            int line = this->tokenizer.line;

            this->eat("while");

            this->eat("(");
            sh_ptr<AST> test = this->Expression();
            this->eat(")");

            sh_ptr<AST> body = this->Statement();

            return sh_ptr<WhileStatementAST>(new WhileStatementAST(line, test, body));
        }

        /**
         * DoWhileStatement
         *  : 'do' Statement 'while' '(' Expression ')'
         *  ;
         */
        sh_ptr<AST> DoWhileStatement()
        {
            int line = this->tokenizer.line;

            this->eat("do");

            sh_ptr<AST> body = this->Statement();

            this->eat("while");
            this->eat("(");
            sh_ptr<AST> test = this->Expression();
            this->eat(")");

            return sh_ptr<DoWhileStatementAST>(new DoWhileStatementAST(line, body, test));
        }

        /**
         * IfStatement
         *  : 'if' '(' Expression ')' Statement
         *  | 'if' '(' Expression ')' Statement 'else' Statement
         */
        sh_ptr<AST> IfStatement()
        {
            int line = this->tokenizer.line;

            this->eat("if");

            this->eat("(");
            sh_ptr<AST> test = this->Expression();
            this->eat(")");

            sh_ptr<AST> consequent = this->Statement();

            sh_ptr<AST> alternate;

            if(this->lookahead.type == "else")
            {
                eat("else");
                alternate = this->Statement();
            }   
            
            return sh_ptr<IfStatementAST>(new IfStatementAST(line, test, consequent, alternate));
        }

        /**
         * VariableStatement
         *  : 'let' Identifier OptVariableDeclaration ';'
         *  ;
         */
        sh_ptr<AST> VariableStatement()
        {
            int line = this->tokenizer.line;

            this->eat("let");
                        
            std::string name = this->eat("IDENTIFIER").value;
            
            sh_ptr<AST> value = this->lookahead.type == "SIMPLE_ASSIGN" ? VariableDeclaration() : sh_ptr<AST>();

            this->eat(";");

            return sh_ptr<VariableStatementAST>(new VariableStatementAST(line, name, value));
        }

        /**
         * VariableDeclaration
         *  : SIMPLE_ASSIGN Expression 
         *  ;
         */
        sh_ptr<AST> VariableDeclaration()
        {
            eat("SIMPLE_ASSIGN");

            return Expression();
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
         *  : AssignmentExpression
         *  ;
         */
        sh_ptr<AST> Expression()
        {
            return this->AssignmentExpression();
        }

        /**
         * AssignmentExpression
         *  : EqualityExpression
         *  | LeftHandSideExpression AssignmentOperator AssignmentExpression
         *  ;
         */
        sh_ptr<AST> AssignmentExpression()
        {
            int line = this->tokenizer.line;

            sh_ptr<AST> left = this->LogicalORExpression();

            if(!this->isAssignmentOperator(this->lookahead.type))
            {
                return left;
            }

            return sh_ptr<AssignmentExpressionAST>(new AssignmentExpressionAST(
                line,
                this->AssignmentOperator().value,
                this->checkValidAssignmentTarget(left),
                AssignmentExpression()
            ));
        }

        /**
         * AssignmentOperator
         *  : SIMPLE_ASSIGN
         *  | COMPLEX_ASSIGN
         *  ;
         */
        Token AssignmentOperator() 
        {
            if (this->lookahead.type == "SIMPLE_ASSIGN")
            {
                return this->eat("SIMPLE_ASSIGN");
            }

            return this->eat("COMPLEX_ASSIGN");
        }

        /**
         * LogicalORExpression
         *  : RelationalExpression
         *  | RelationalExpression EQUALITY_OPERATOR RelationalExpression
         *  ;
         */
        sh_ptr<AST> LogicalORExpression() 
        {
            return this->LogicalExpression(&Parser::LogicalANDExpression, "LOGICAL_OR");
        }

        /**
         * LogicalANDExpression
         *  : RelationalExpression
         *  | RelationalExpression EQUALITY_OPERATOR RelationalExpression
         *  ;
         */
        sh_ptr<AST> LogicalANDExpression() 
        {
            return this->LogicalExpression(&Parser::EqualityExpression, "LOGICAL_AND");
        }

        /**
         * EqualityExpression
         *  : RelationalExpression
         *  | RelationalExpression EQUALITY_OPERATOR RelationalExpression
         *  ;
         */
        sh_ptr<AST> EqualityExpression() 
        {
            return this->BinaryExpression(&Parser::RelationalExpression, "EQUALITY_OPERATOR");
        }

        /**
         * RelationalExpression
         *  : AdditiveExpression
         *  | AdditiveExpression RELATIONAL_OPERATOR RelationalExpression
         *  ;
         */
        sh_ptr<AST> RelationalExpression() 
        {
            return this->BinaryExpression(&Parser::AdditiveExpression, "RELATIONAL_OPERATOR");
        }

        /**
         * LeftHandSideExpression
         *  : Identifier    
         *  ;
         */
        sh_ptr<AST> LeftHandSideExpression()
        {
            return Identifier();
        }

        /**
         * Identifier
         *  : IDENTIFIER    
         *  ;
         */
        sh_ptr<AST> Identifier()
        {
            int line = this->tokenizer.line;

            std::string name = this->eat("IDENTIFIER").value;

            return sh_ptr<IdentifierAST>(new IdentifierAST(line, name));
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
            return this->BinaryExpression(&Parser::PrimaryExpression, "MULTIPLICATIVE_OPERATOR");
        }

        /**
         * PrimaryExpression
         *  : Literal
         *  | ParenthesizedExpression
         *  | LeftHandSideExpression
         *  ;
         */
        sh_ptr<AST> PrimaryExpression()
        {
            if (this->isLiteral(this->lookahead.type)) 
            {
                return this->Literal();
            }

            if (this->lookahead.type == "(")
            {
                return this->ParenthesizedExpression();
            }

            return this->LeftHandSideExpression();
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
         *  | HexLiteral
         *  ;
         */
        sh_ptr<AST> Literal()
        {
            if (lookahead.type == "NUMBER")
                return this->NumericLiteral();

            if (lookahead.type == "HEX")
                return this->HexLiteral();

            if (lookahead.type == "STRING")
                return this->StringLiteral();

            if (lookahead.type == "true")
                return this->BooleanLiteral(true);

            if (lookahead.type == "false")
                return this->BooleanLiteral(false);
                                                        
            error("unexpected literal production");
            return sh_ptr<AST>();
        }

        /**
         * NumericLiteral
         *  : NUMBER
         *  ;
         */
        sh_ptr<AST> NumericLiteral()
        {
            int line = this->tokenizer.line;

            Token token = this->eat("NUMBER");

            return sh_ptr<NumericLiteralAST>(new NumericLiteralAST(line, std::stod(token.value)));
        }

        /**
         * BooleanLiteral
         *  : true
         *  | false
         *  ;
         */
        sh_ptr<AST> BooleanLiteral(bool value)
        {
            int line = this->tokenizer.line;

            eat(value ? "true" : "false");

            return sh_ptr<BooleanLiteralAST>(new BooleanLiteralAST(line, value));
        }

        /**
         * NullLiteral
         *  : null
         *  ;
         */
        sh_ptr<AST> NullLiteral()
        {
            int line = this->tokenizer.line;

            eat("null");

            return sh_ptr<NullLiteralAST>(new NullLiteralAST(line));
        }

        /**
         * HexLiteral
         *  :
         *  HEX
         *  ;
         */
        sh_ptr<AST> HexLiteral()
        {
            int line = this->tokenizer.line;

            Token token = this->eat("HEX");
            
            return sh_ptr<NumericLiteralAST>(new NumericLiteralAST(line, std::stol(token.value, 0, 16)));
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

            return sh_ptr<StringLiteralAST>(new StringLiteralAST(line, token.value.substr(1, token.value.size() - 2)));
        }

        /**
         * Expects a token of a given type.
         */

        Token eat(const std::string &tokenType)
        {
            Token token = this->lookahead;

            if (token.type == "EOF")
            {
                error("unexpected end of input, expected: " + tokenType);
                return Token();
            }

            if (token.type != tokenType)
            {
                error("unexpected token: \"" + token.value + "\", expected: " + tokenType);
                return Token();
            }

            // Advance to next token.
            this->lookahead = this->tokenizer.getNextToken();

            return token;
        }
    };
}

#endif