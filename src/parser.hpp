#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

#include "tokens.hpp"
#include "tokenizer.hpp"
#include "ast.hpp"
#include "aliases.hpp"
#include "util.hpp"

namespace eris
{
    /**
     * Eris parser: recursive descent implementation.
     */

    class Parser
    {
    public:
        /**
         * @brief
         * Initializes the parser.
         */
        Parser() : string(""), tokenizer(Tokenizer()) {}

        void error(const std::string &message) const
        {
            throw std::string(std::to_string(this->tokenizer.line) + ": syntax error: " + message);
        }

        /**
         * @brief
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
         * @brief
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
         * @brief
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
         * @brief
         * Statement
         *  : ExpressionStatement
         *  | EmptyStatement
         *  | BlockStatement
         *  | VariableStatement
         *  | IterationStatement
         *  | ReturnStatement
         *  | FunctionDeclaration
         *  | ClassDeclaration
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

            if (this->lookahead.type == "while" || this->lookahead.type == "do" || this->lookahead.type == "for")
            {
                return this->IterationStatement();
            }

            if (this->lookahead.type == "return")
            {
                return this->ReturnStatement();
            }

            if (this->lookahead.type == "def")
            {
                return this->FunctionDeclaration();
            }

            if (this->lookahead.type == "class")
            {
                return this->ClassDeclaration();
            }

            if (this->lookahead.type == "namespace")
            {
                return this->NamespaceDeclaration();
            }

            return this->ExpressionStatement();
        }

        /**
         * @brief
         * ClassDeclaration
         *  : 'class' Identifier OptClassExtends BlockStatement
         *  ;
         */
        sh_ptr<AST> ClassDeclaration()
        {
            int line = this->tokenizer.line;

            this->eat("class");

            std::string name = this->eat("IDENTIFIER").value;

            sh_ptr<AST> superClass;

            if (this->lookahead.type == "extends")
            {
                superClass = this->ClassExtends();
            }

            sh_ptr<AST> body = this->BlockStatement();

            return sh_ptr<ClassDeclarationAST>(new ClassDeclarationAST(line, name, superClass, body));
        }

        /**
         * @brief
         * ClassExtends
         *  : 'extends' Expression
         *  ;
         */
        sh_ptr<AST> ClassExtends()
        {
            this->eat("extends");
            return this->Expression();
        }

        /**
         * @brief
         * NamespaceDeclaration
         *  : 'namespace' Identifier BlockStatement
         *  ;
         */
        sh_ptr<AST> NamespaceDeclaration()
        {
            int line = this->tokenizer.line;

            this->eat("namespace");

            std::string name = this->eat("IDENTIFIER").value;

            sh_ptr<AST> body = this->BlockStatement();

            return sh_ptr<NamespaceDeclarationAST>(new NamespaceDeclarationAST(line, name, body));
        }

        /**
         * @brief
         * FunctionDeclaration
         *  : 'def' Identifier '(' OptFormalParameterList ')' BlockStatement
         *  ;
         */
        sh_ptr<AST> FunctionDeclaration()
        {
            int line = this->tokenizer.line;

            this->eat("def");

            std::string name = this->eat("IDENTIFIER").value;

            this->eat("(");

            std::vector<sh_ptr<AST> > params;

            if (this->lookahead.type != ")")
            {
                params = this->FormalParameterList();
            }

            this->eat(")");

            sh_ptr<AST> body = this->BlockStatement();

            return sh_ptr<FunctionDeclarationAST>(new FunctionDeclarationAST(line, name, params, body));
        }

        /**
         * @brief
         * FormalParameterList
         *  : Identifier
         *  | FormalParameterList ',' Identifier
         *  ;
         */
        std::vector<sh_ptr<AST> > FormalParameterList()
        {
            std::vector<sh_ptr<AST> > params;

            for (;;)
            {
                params.push_back(this->Identifier());

                if (this->lookahead.type != ",")
                {
                    break;
                }

                this->eat(",");
            }

            return params;
        }

        /**
         * @brief
         * ReturnStatement
         *  : 'return' Expression ';'
         *  ;
         */
        sh_ptr<AST> ReturnStatement()
        {
            int line = this->tokenizer.line;
            this->eat("return");
            sh_ptr<AST> argument;

            if (this->lookahead.type != ";")
            {
                argument = this->Expression();
            }

            this->eat(";");
            return sh_ptr<AST>(new ReturnStatementAST(line, argument));
        }

        /**
         * @brief
         * IterationStatement
         *  : WhileStatement
         *  | DoWhileStatement
         *  ;
         *
         */
        sh_ptr<AST> IterationStatement()
        {
            if (this->lookahead.type == "while")
            {
                return this->WhileStatement();
            }

            if (this->lookahead.type == "do")
            {
                return this->DoWhileStatement();
            }

            if (this->lookahead.type == "for")
            {
                return this->ForStatement();
            }

            return sh_ptr<AST>();
        }

        /**
         * @brief
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
         * @brief
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
         * @brief
         * ForStatementInit
         *  : VariableStatementInit
         *  | Expression
         *  ;
         */
        sh_ptr<AST> ForStatementInit()
        {
            if (this->lookahead.type == "let")
            {
                return this->VariableStatementInit();
            }

            return this->Expression();
        }

        /**
         * @brief
         * ForStatement
         *  : 'for' '(' ForStatementInit ';' Expression ';' Expression ')'
         *  ;
         */
        sh_ptr<AST> ForStatement()
        {
            int line = this->tokenizer.line;

            this->eat("for");

            this->eat("(");

            sh_ptr<AST> init = this->ForStatementInit();
            this->eat(";");
            sh_ptr<AST> test = this->Expression();
            this->eat(";");
            sh_ptr<AST> update = this->Expression();

            this->eat(")");

            sh_ptr<AST> body = this->Statement();

            return sh_ptr<ForStatementAST>(new ForStatementAST(line, init, test, update, body));
        }

        /**
         * @brief
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

            if (this->lookahead.type == "else")
            {
                eat("else");
                alternate = this->Statement();
            }

            return sh_ptr<IfStatementAST>(new IfStatementAST(line, test, consequent, alternate));
        }

        /**
         * @brief
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
         * @brief
         * VariableStatementInit
         *  : 'let' Identifier OptVariableDeclaration
         *  ;
         */
        sh_ptr<AST> VariableStatementInit()
        {
            int line = this->tokenizer.line;

            this->eat("let");

            std::string name = this->eat("IDENTIFIER").value;

            sh_ptr<AST> value = this->lookahead.type == "SIMPLE_ASSIGN" ? VariableDeclaration() : sh_ptr<AST>();

            return sh_ptr<VariableStatementAST>(new VariableStatementAST(line, name, value));
        }

        /**
         * @brief
         * VariableStatement
         *  : VariableStatementInit ';'
         *  ;
         */
        sh_ptr<AST> VariableStatement()
        {
            sh_ptr<AST> result = this->VariableStatementInit();

            this->eat(";");

            return result;
        }

        /**
         * @brief
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
         * @brief
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
         * @brief
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
         * @brief
         * Expression
         *  : AssignmentExpression
         *  ;
         */
        sh_ptr<AST> Expression()
        {
            return this->AssignmentExpression();
        }

        /**
         * @brief
         * AssignmentExpression
         *  : EqualityExpression
         *  | LeftHandSideExpression AssignmentOperator AssignmentExpression
         *  ;
         */
        sh_ptr<AST> AssignmentExpression()
        {
            int line = this->tokenizer.line;

            sh_ptr<AST> left = this->LogicalORExpression();

            if (!this->isAssignmentOperator(this->lookahead.type))
            {
                return left;
            }

            return sh_ptr<AssignmentExpressionAST>(new AssignmentExpressionAST(
                line,
                this->AssignmentOperator().value,
                this->checkValidAssignmentTarget(left),
                AssignmentExpression()));
        }

        /**
         * @brief
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
         * @brief
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
         * @brief
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
         * @brief
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
         * @brief
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
         * @brief
         * LeftHandSideExpression
         *  : Identifier
         *  ;
         */
        sh_ptr<AST> LeftHandSideExpression()
        {
            return Identifier();
        }

        /**
         * @brief
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
         * @brief
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
         * @brief
         * MultiplicativeExpression
         *  : UnaryExpression
         *  | MultiplicativeExpression MULTIPLICATIVE_OPERATOR UnaryExpression
         *  ;
         */
        sh_ptr<AST> MultiplicativeExpression()
        {
            return this->BinaryExpression(&Parser::UnaryExpression, "MULTIPLICATIVE_OPERATOR");
        }

        /**
         * @brief
         * UnaryExpression
         *  : CallMemberExpression
         *  | ADDITIVE_OPERATOR CallMemberExpression
         *  | LOGICAL_NOT CallMemberExpression
         *  ;
         */
        sh_ptr<AST> UnaryExpression()
        {
            int line = this->tokenizer.line;

            if (this->lookahead.type == "ADDITIVE_OPERATOR")
            {
                std::string op = this->lookahead.value;
                this->eat("ADDITIVE_OPERATOR");
                return sh_ptr<UnaryExpressionAST>(new UnaryExpressionAST(line, op, this->CallMemberExpression()));
            }

            if (this->lookahead.type == "LOGICAL_NOT")
            {
                std::string op = this->lookahead.value;
                this->eat("LOGICAL_NOT");
                return sh_ptr<UnaryExpressionAST>(new UnaryExpressionAST(line, op, this->CallMemberExpression()));
            }

            return this->CallMemberExpression();
        }

        /**
         * @brief
         * CallMemberExpression
         *  : MemberExpression
         *  | CallExpression
         *  ;
         */
        sh_ptr<AST> CallMemberExpression()
        {
            // Member part, might be part of a call:
            sh_ptr<AST> member = this->MemberExpression();

            // See if we have a call expression:
            if (this->lookahead.type == "(")
            {
                return this->CallExpression(member);
            }

            return member;
        }

        /**
         * @brief
         * MemberExpression
         *  : PrimaryExpression
         *  | MemberExpression '.' Identifier
         *  | MemberExpression '::' Identifier
         *  | LeftHandSideExpression
         *  ;
         */
        sh_ptr<AST> MemberExpression()
        {
            int line = this->tokenizer.line;
            sh_ptr<AST> object = this->PrimaryExpression();

            while (this->lookahead.type == "." || this->lookahead.type == "::")
            {
                if (this->lookahead.type == ".")
                {
                    this->eat(".");
                    sh_ptr<AST> property = this->Identifier();
                    object = sh_ptr<MemberExpressionAST>(new MemberExpressionAST(line, object, property, "dot"));
                }
                else if (this->lookahead.type == "::")
                {
                    this->eat("::");
                    sh_ptr<AST> property = this->Identifier();
                    object = sh_ptr<MemberExpressionAST>(new MemberExpressionAST(line, object, property, "scopeResolution"));
                }
            }

            return object;
        }

        /**
         * @brief
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

        /**
         * @brief
         *
         *
         */

        sh_ptr<AST> ParenthesizedExpression()
        {
            eat("(");

            sh_ptr<AST> expression = this->Expression();

            eat(")");

            return expression;
        }

        /**
         * Literal
         *  : IntLiteral
         *  | DoubleLiteral
         *  | StringLiteral
         *  | BooleanLiteral
         *  ;
         */
        sh_ptr<AST> Literal()
        {
            if (lookahead.type == "INT")
            {
                return this->IntLiteral();
            }

            if (lookahead.type == "DOUBLE")
            {
                return this->DoubleLiteral();
            }

            if (lookahead.type == "STRING")
            {
                return this->StringLiteral();
            }

            if (lookahead.type == "true")
            {
                return this->BooleanLiteral(true);
            }

            if (lookahead.type == "false")
            {
                return this->BooleanLiteral(false);
            }

            if (lookahead.type == "[")
            {
                return this->ListLiteral();
            }

            error("unexpected literal production");
            return sh_ptr<AST>();
        }

        /**
         * IntLiteral
         *  : INT
         *  ;
         */
        sh_ptr<AST> IntLiteral()
        {
            int line = this->tokenizer.line;

            Token token = this->eat("INT");

            return sh_ptr<IntLiteralAST>(new IntLiteralAST(line, (std::int32_t)std::stol(token.value)));
        }

        /**
         * DoubleLiteral
         *  : DOUBLE
         *  ;
         */
        sh_ptr<AST> DoubleLiteral()
        {
            int line = this->tokenizer.line;

            Token token = this->eat("DOUBLE");

            return sh_ptr<DoubleLiteralAST>(new DoubleLiteralAST(line, std::stod(token.value)));
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
         * @brief
         * ListLiteral
         *  : '[' OptArgumentList ']'
         *  ;
         */
        sh_ptr<AST> ListLiteral()
        {
            int line = this->tokenizer.line;

            this->eat("[");

            std::vector<sh_ptr<AST> > elements;

            if (this->lookahead.type != "]")
            {
                elements = this->ArgumentList();
            }

            this->eat("]");

            return sh_ptr<ListLiteralAST>(new ListLiteralAST(line, elements));
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

    protected:
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
         * @brief
         * Generic logical expression.
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
            if (node->type() == "Identifier" || node->type() == "MemberExpression")
            {
                return node;
            }

            error("invalid left-hand side in assignment expression");
            return sh_ptr<AST>();
        }

        /**
         * @brief
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
            return tokenType == "INT" || tokenType == "DOUBLE" || tokenType == "STRING" || tokenType == "true" || tokenType == "false" || tokenType == "null" || tokenType == "[";
        }

        /**
         * @brief
         * Generic call expression helper.
         *
         * CallExpression
         *  : Callee Arguments
         *  ;
         *
         * Callee:
         *  : MemberExpression
         *  | CallExpression
         */
        sh_ptr<AST> CallExpression(sh_ptr<AST> callee)
        {
            int line = this->tokenizer.line;

            sh_ptr<AST> callExpression(new CallExpressionAST(line, callee, this->Arguments()));

            if (this->lookahead.type == "(")
            {
                callExpression = this->CallExpression(callExpression);
            }

            return callExpression;
        }

        /**
         * @brief
         * Arguments
         *  : '(' OptArgumentList ')'
         *  ;
         */
        std::vector<sh_ptr<AST> > Arguments()
        {
            this->eat("(");

            std::vector<sh_ptr<AST> > argumentList;

            if (this->lookahead.type != ")")
            {
                argumentList = this->ArgumentList();
            }

            this->eat(")");

            return argumentList;
        }

        /**
         * @brief
         * ArgumentList
         *  : AssignmentExpression
         *  | ArgumentList ',' AssignmentExpression
         *  ;
         */
        std::vector<sh_ptr<AST> > ArgumentList()
        {
            std::vector<sh_ptr<AST> > params;

            for (;;)
            {
                params.push_back(this->AssignmentExpression());

                if (this->lookahead.type != ",")
                {
                    break;
                }

                this->eat(",");
            }

            return params;
        }
    };
}

#endif