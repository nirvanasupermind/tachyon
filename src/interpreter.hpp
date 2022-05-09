#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "values.hpp"
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
            throw std::string(std::to_string(line) + ": runtime error: " + message);
        }

        sh_ptr<Value> eval(std::vector<sh_ptr<AST> > statementList, sh_ptr<Environment> env)
        {
            sh_ptr<Value> result;

            for (std::size_t i = 0; i < statementList.size(); i++)
            {
                result = this->eval(statementList.at(i), env);
            }

            return result;
        }

        sh_ptr<Value> eval(sh_ptr<AST> exp, sh_ptr<Environment> env)
        {
            return this->eval(exp.get(), env);
        }

        sh_ptr<Value> eval(AST *exp, sh_ptr<Environment> env)
        {
            std::string type = exp->type();

            // --------------------------------------------
            // Literals:

            if (type == "NumericLiteral")
            {
                return this->eval(dynamic_cast<NumericLiteralAST *>(exp), env);
            }

            if (type == "StringLiteral")
            {
                return this->eval(dynamic_cast<StringLiteralAST *>(exp), env);
            }

            if (type == "BooleanLiteral")
            {
                return this->eval(dynamic_cast<BooleanLiteralAST *>(exp), env);
            }

            if (type == "NullLiteral")
            {
                return this->eval(dynamic_cast<NullLiteralAST *>(exp), env);
            }

            // --------------------------------------------
            // Expressions:

            if (type == "AssignmentExpression")
            {
                return this->eval(dynamic_cast<AssignmentExpressionAST *>(exp), env);
            }

            if (type == "Identifier")
            {
                return this->eval(dynamic_cast<IdentifierAST *>(exp), env);
            }

            if (type == "BinaryExpression")
            {
                return this->eval(dynamic_cast<BinaryExpressionAST *>(exp), env);
            }

            if (type == "MemberExpression")
            {
                return this->eval(dynamic_cast<MemberExpressionAST *>(exp), env);
            }

            // --------------------------------------------
            // Statements:

            if (type == "EmptyStatement")
            {
                return this->eval(dynamic_cast<EmptyStatementAST *>(exp), env);
            }

            if (type == "BlockStatement")
            {
                return this->eval(dynamic_cast<BlockStatementAST *>(exp), env);
            }

            if (type == "ExpressionStatement")
            {
                return this->eval(dynamic_cast<ExpressionStatementAST *>(exp), env);
            }

            if (type == "VariableStatement")
            {
                return this->eval(dynamic_cast<VariableStatementAST *>(exp), env);
            }

            if (type == "IfStatement")
            {
                return this->eval(dynamic_cast<IfStatementAST *>(exp), env);
            }

            if (type == "WhileStatement")
            {
                return this->eval(dynamic_cast<WhileStatementAST *>(exp), env);
            }

            if (type == "DoWhileStatement")
            {
                return this->eval(dynamic_cast<DoWhileStatementAST *>(exp), env);
            }

            if (type == "ForStatement")
            {
                return this->eval(dynamic_cast<ForStatementAST *>(exp), env);
            }

            if (type == "PrintStatement")
            {
                return this->eval(dynamic_cast<PrintStatementAST *>(exp), env);
            }

            if (type == "FunctionDeclaration")
            {
                return this->eval(dynamic_cast<FunctionDeclarationAST *>(exp), env);
            }

            // --------------------------------------------
            // Unimplemented:

            error(exp->line, "unimplemented");
            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(FunctionDeclarationAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Function> fn = sh_ptr<Function>(new Function(exp->params, exp->body, env));

            env->define(exp->name, fn);

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(MemberExpressionAST *exp, sh_ptr<Environment> env)
        {
            if (exp->computed)
            {
                return sh_ptr<Value>();
            }

            sh_ptr<Value> value = eval(exp->object, env);

            sh_ptr<Object> object = std::dynamic_pointer_cast<Object>(value);

            std::cout << "dbg 175" << '\n';

            if (!object)
            {
                std::cout << "dbg 179" << '\n';

                error(exp->object->line, value->str() + " does not support member access");
            }
            std::cout << "dbg 183" << '\n';

            std::string property = std::dynamic_pointer_cast<IdentifierAST>(exp->property)->name;

            std::cout << "dbg 187" << '\n';

            try
            {
                std::cout << "dbg 192 " << object->members << '\n';

                return object->members->lookup(property);
            }
            catch (const std::string &e)
            {
                std::cout << "dbg 198" << '\n';
                // Error messages thrown from outside of interpreter don't have a line number
                // so insert one here

                error(exp->line, e);
                return sh_ptr<Value>();
            }
        }

        sh_ptr<Value> eval(EmptyStatementAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(BlockStatementAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Environment> child_env(new Environment());
            child_env->parent = env;

            return this->eval(exp->body, child_env);
        }

        sh_ptr<Value> eval(ExpressionStatementAST *exp, sh_ptr<Environment> env)
        {
            return this->eval(exp->expression, env);
        }

        sh_ptr<Value> eval(VariableStatementAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Value> value;
            if (exp->value)
            {
                value = this->eval(exp->value, env);
            }
            else
            {
                value = sh_ptr<Null>(new Null());
            }

            env->define(exp->name, value);

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(IfStatementAST *exp, sh_ptr<Environment> env)
        {
            if (this->eval(exp->test, env)->truthy())
            {
                this->eval(exp->consequent, env);
            }
            else
            {
                if (exp->alternate)
                {
                    return this->eval(exp->alternate, env);
                }
            }

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(WhileStatementAST *exp, sh_ptr<Environment> env)
        {
            while (this->eval(exp->test, env)->truthy())
            {
                this->eval(exp->body, env);
            }

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(DoWhileStatementAST *exp, sh_ptr<Environment> env)
        {
            do
            {
                this->eval(exp->body, env);
            } while (this->eval(exp->test, env)->truthy());

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(ForStatementAST *exp, sh_ptr<Environment> env)
        {
            this->eval(exp->init, env);

            while (this->eval(exp->test, env)->truthy())
            {
                this->eval(exp->body, env);
                this->eval(exp->update, env);
            }

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(PrintStatementAST *exp, sh_ptr<Environment> env)
        {
            std::cout << this->eval(exp->argument, env)->str() << '\n';

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(AssignmentExpressionAST *exp, sh_ptr<Environment> env)
        {
            std::string name = std::dynamic_pointer_cast<IdentifierAST>(exp->left)->name;
            sh_ptr<Number> value = std::dynamic_pointer_cast<Number>(this->eval(exp->right, env));

            if (exp->op == "=")
            {
                env->assign(name, value);

                return sh_ptr<Value>();
            }

            sh_ptr<Number> currentValue;

            try
            {
                currentValue = std::dynamic_pointer_cast<Number>(env->lookup(name));
            }
            catch (const std::string &e)
            {
                // Error messages thrown from outside of interpreter don't have a line number
                // so insert one here

                error(exp->line, e);
                return sh_ptr<Value>();
            }

            if (!currentValue)
            {
                error(exp->right->line, "invalid argument #1 for binary operator \"" + exp->op + "\"");
                return sh_ptr<Value>();
            }

            if (!value)
            {
                error(exp->right->line, "invalid argument #2 for binary operator \"" + exp->op + "\"");
                return sh_ptr<Value>();
            }

            if (exp->op == "+=")
            {
                env->assign(name, sh_ptr<Number>(new Number(currentValue->value + value->value)));

                return sh_ptr<Value>();
            }

            if (exp->op == "-=")
            {
                env->assign(name, sh_ptr<Number>(new Number(currentValue->value - value->value)));

                return sh_ptr<Value>();
            }

            if (exp->op == "*=")
            {
                env->assign(name, sh_ptr<Number>(new Number(currentValue->value * value->value)));

                return sh_ptr<Value>();
            }

            if (exp->op == "/=")
            {
                env->assign(name, sh_ptr<Number>(new Number(currentValue->value / value->value)));

                return sh_ptr<Value>();
            }

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(IdentifierAST *exp, sh_ptr<Environment> env)
        {
            try
            {
                return env->lookup(exp->name);
            }
            catch (const std::string &e)
            {
                // Error messages thrown from outside of interpreter don't have a line number
                // so insert one here

                error(exp->line, e);
                return sh_ptr<Value>();
            }
        }

        sh_ptr<Value> eval(BinaryExpressionAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Number> left = std::dynamic_pointer_cast<Number>(this->eval(exp->left, env));
            sh_ptr<Number> right = std::dynamic_pointer_cast<Number>(this->eval(exp->right, env));

            if (!left)
            {
                error(exp->left->line, "invalid argument #1 for binary operator \"" + exp->op + "\"");
                return sh_ptr<Value>();
            }

            if (!right)
            {
                error(exp->right->line, "invalid argument #2 for binary operator \"" + exp->op + "\"");
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

            if (exp->op == ">")
            {
                return sh_ptr<Boolean>(new Boolean(left->value > right->value));
            }

            if (exp->op == ">=")
            {
                return sh_ptr<Boolean>(new Boolean(left->value >= right->value));
            }

            if (exp->op == "<")
            {
                return sh_ptr<Boolean>(new Boolean(left->value < right->value));
            }

            if (exp->op == "<=")
            {
                return sh_ptr<Boolean>(new Boolean(left->value <= right->value));
            }

            if (exp->op == "==")
            {
                return sh_ptr<Boolean>(new Boolean(left->value == right->value));
            }

            if (exp->op == "!=")
            {
                return sh_ptr<Boolean>(new Boolean(left->value != right->value));
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

        sh_ptr<Value> eval(BooleanLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<Boolean>(new Boolean(exp->value));
        }

        sh_ptr<Value> eval(NullLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<Value>();
        }
    };
}

#endif