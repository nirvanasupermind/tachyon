#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "values.hpp"
#include "environment.hpp"
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

        Interpreter(sh_ptr<Environment> global = sh_ptr<Environment>())
            : global(global)
        {
        }

        sh_ptr<Value> eval(sh_ptr<AST> exp, sh_ptr<Environment> env)
        {
            return eval(exp.get(), env);
        }

        sh_ptr<Value> eval(AST *exp, sh_ptr<Environment> env)
        {
            switch (exp->type())
            {
            case ASTType::EmptyStatement:
                return sh_ptr<None>(new None());
            case ASTType::BlockStatement:
                return eval(dynamic_cast<BlockStatementAST *>(exp), env);
            case ASTType::ExpressionStatement:
                return eval(dynamic_cast<ExpressionStatementAST *>(exp), env);
            case ASTType::NumericLiteral:
                return eval(dynamic_cast<NumericLiteralAST *>(exp), env);
            case ASTType::StringLiteral:
                return eval(dynamic_cast<StringLiteralAST *>(exp), env);
            default:
                throw std::string("Unimplemented");
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