#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "values.hpp"
// #include "environment.hpp"
#include "aliases.hpp"

namespace eris
{
    /**
     * Eris interpreter.
     */
    class Interpreter
    {
    public:
        sh_ptr<Environment> global;

        Interpreter(sh_ptr<Environment> global = sh_ptr<Environment>(new Environment()))
            : global(global)
        {
        }

        void error(int line, const std::string &message) const
        {
            throw std::string(std::to_string(line) + ": runtime error: "+ message);
        }

        sh_ptr<Value> eval(sh_ptr<AST> exp, sh_ptr<Environment> env)
        {
            return eval(exp.get(), env);
        }

        sh_ptr<Value> eval(AST *exp, sh_ptr<Environment> env)
        {
            switch (exp->type())
            {
            // --------------------------------------------
            // Statements:

            case ASTType::EmptyStatement:
                return sh_ptr<None>(new None());
            case ASTType::VariableStatement:
                return eval(dynamic_cast<VariableStatementAST *>(exp), env);
            case ASTType::BlockStatement:
                return eval(dynamic_cast<BlockStatementAST *>(exp), env);
            case ASTType::ExpressionStatement:
                return eval(dynamic_cast<ExpressionStatementAST *>(exp), env);

            // --------------------------------------------
            // Expressions:

            case ASTType::Identifier:
                return eval(dynamic_cast<IdentifierAST *>(exp), env);
            case ASTType::BinaryExpression:
                return eval(dynamic_cast<BinaryExpressionAST *>(exp), env);

            // --------------------------------------------
            // Literals:

            case ASTType::NumericLiteral:
                return eval(dynamic_cast<NumericLiteralAST *>(exp), env);
            case ASTType::StringLiteral:
                return eval(dynamic_cast<StringLiteralAST *>(exp), env);

            // --------------------------------------------
            // Unimplemented:

            default:
                error(exp->line, "unimplemented: " + exp->str());
                return sh_ptr<Value>();
            }
        }

        sh_ptr<Value> eval(std::vector<sh_ptr<AST> > statementList, sh_ptr<Environment> env)
        {
            if(statementList.size() == 0) 
            {
                return sh_ptr<None>(new None());
            }

            for (std::size_t i = 0; i < statementList.size(); i++)
            {
                eval(statementList.at(i), env);
            }

            return eval(statementList.back(), env);
        }

        sh_ptr<Value> eval(VariableStatementAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Value> value = this->eval(exp->value, env);

            env->define(exp->name, this->eval(exp->value, env));

            return value;
        }

        sh_ptr<Value> eval(BlockStatementAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Environment> child_env(new Environment());
            child_env->parent = env;

            return eval(exp->body, child_env);
        }

        sh_ptr<Value> eval(ExpressionStatementAST *exp, sh_ptr<Environment> env)
        {
            return eval(exp->expression, env);
        }

        sh_ptr<Value> eval(IdentifierAST *exp, sh_ptr<Environment> env)
        {
            try
            {
                return env->lookup(exp->name);
            }
            catch(const std::string &s)
            {
                error(exp->line, s);
                return sh_ptr<Value>();
            }            
        }

        sh_ptr<Value> eval(BinaryExpressionAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Number> left = std::dynamic_pointer_cast<Number>(eval(exp->left, env));
            sh_ptr<Number> right = std::dynamic_pointer_cast<Number>(eval(exp->right, env));

            if(!left) 
            {
                error(exp->left->line, "invalid argument #1 for binary operator \""+exp->op+"\"");
                return sh_ptr<Value>();
            }

            if(!right) 
            {
                error(exp->right->line, "invalid argument #2 for binary operator \""+exp->op+"\"");
                return sh_ptr<Value>();
            }

            if (exp->op == "+")
            {
                return sh_ptr<Number>(new Number(left->value + right->value));
            }

            if (exp->op == "-")
            {
                return sh_ptr<Number>(new Number(left->value - right->value));
            }

            if (exp->op == "*")
            {
                return sh_ptr<Number>(new Number(left->value * right->value));
            }

            if (exp->op == "/")
            {
                return sh_ptr<Number>(new Number(left->value / right->value));
            }


            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(NumericLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<Number>(new Number(exp->value));
        }

        sh_ptr<Value> eval(StringLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<String>(new String(exp->string));
        }
    };
}

#endif