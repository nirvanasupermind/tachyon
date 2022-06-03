#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "values.hpp"
#include "aliases.hpp"
#include "builtins.hpp"

namespace eris
{
    /**
     * @brief
     * Eris interpreter.
     */
    class Interpreter
    {
    public:
        sh_ptr<Environment> global;

        sh_ptr<Value> returnValue;

        Interpreter(sh_ptr<Environment> global = sh_ptr<Environment>(new Environment()))
            : global(global)
        {
        }

        void error(int line, const std::string &message) const
        {
            throw std::string(std::to_string(line) + ": runtime error: " + message);
        }

        sh_ptr<Value> eval(const std::vector<sh_ptr<AST> > &statementList, sh_ptr<Environment> env, bool isInFunction = false)
        {
            if (statementList.size() == 0)
            {
                return sh_ptr<Value>();
            }

            sh_ptr<Value> result;

            for (sh_ptr<AST> statement : statementList)
            {
                result = this->eval(statement, env);

                if (this->returnValue)
                {
                    sh_ptr<Value> temp = this->returnValue;

                    if (isInFunction)
                    {
                        this->returnValue = sh_ptr<Value>();
                    }

                    return temp;
                }
            }

            return sh_ptr<Value>();
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

            if (type == "IntLiteral")
            {
                return this->eval(dynamic_cast<IntLiteralAST *>(exp), env);
            }

            if (type == "DoubleLiteral")
            {
                return this->eval(dynamic_cast<DoubleLiteralAST *>(exp), env);
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

            if (type == "CallExpression")
            {
                return this->eval(dynamic_cast<CallExpressionAST *>(exp), env);
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

            if (type == "ReturnStatement")
            {
                return this->eval(dynamic_cast<ReturnStatementAST *>(exp), env);
            }

            if (type == "FunctionDeclaration")
            {
                return this->eval(dynamic_cast<FunctionDeclarationAST *>(exp), env);
            }

            if (type == "ClassDeclaration")
            {
                return this->eval(dynamic_cast<ClassDeclarationAST *>(exp), env);
            }

            if (type == "NamespaceDeclaration")
            {
                return this->eval(dynamic_cast<NamespaceDeclarationAST *>(exp), env);
            }

            // --------------------------------------------
            // Unimplemented:

            error(exp->line, "unimplemented");
            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(NamespaceDeclarationAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Environment> namespaceEnv(new Environment({}, env));

            std::vector<sh_ptr<AST> > body = std::dynamic_pointer_cast<BlockStatementAST>(exp->body)->body;
            this->eval(body, namespaceEnv);

            env->define(exp->name, sh_ptr<Object>(new Object(namespaceEnv)));

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(ClassDeclarationAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Environment> classEnv(new Environment());
            if (exp->superClass)
            {
                sh_ptr<Value> superValue = this->eval(exp->superClass, env);
                sh_ptr<Class> superClass = std::dynamic_pointer_cast<Class>(superClass);

                if (!superClass)
                {
                    this->error(exp->superClass->line, superValue->str() + " cannot be used as a superclass");
                    return sh_ptr<Value>();
                }

                classEnv->parent = superClass->members;
            }
            else
            {
                classEnv->parent = env;
            }

            std::vector<sh_ptr<AST> > body = std::dynamic_pointer_cast<BlockStatementAST>(exp->body)->body;
            this->eval(body, classEnv);

            env->define(exp->name, sh_ptr<Class>(new Class(classEnv)));
            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(FunctionDeclarationAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<UserDefinedFunction> fn = sh_ptr<UserDefinedFunction>(new UserDefinedFunction(exp->params, exp->body, env));
            env->define(exp->name, fn);

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(CallExpressionAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Value> callee = this->eval(exp->callee, env);

            sh_ptr<NativeFunction> native = std::dynamic_pointer_cast<NativeFunction>(callee);

            sh_ptr<Class> cls = std::dynamic_pointer_cast<Class>(callee);

            sh_ptr<UserDefinedFunction> userDefined = std::dynamic_pointer_cast<UserDefinedFunction>(callee);

            std::vector<sh_ptr<Value> > args;

            if ((userDefined || native) && exp->callee->type() == "MemberExpression")
            {
                sh_ptr<MemberExpressionAST> member = std::dynamic_pointer_cast<MemberExpressionAST>(exp->callee);
                if (member->subtype == "dot")
                {
                    sh_ptr<Object> object = std::dynamic_pointer_cast<Object>(this->eval(member->object, env));

                    args.push_back(object);
                }
            }

            for (sh_ptr<AST> arg : exp->arguments)
            {
                args.push_back(this->eval(arg, env));
            }

            // 1. Native function:
            if (native)
            {
                return native->fn(args);
            }

            // 2. Class instantiation:
            if (cls)
            {
                sh_ptr<Value> constructor = std::dynamic_pointer_cast<UserDefinedFunction>(cls->members->lookup("constructor"));

                sh_ptr<UserDefinedFunction> constructor_fn = std::dynamic_pointer_cast<UserDefinedFunction>(constructor);

                if (!constructor_fn)
                {
                    this->error(exp->callee->line, constructor->str() + " is not callable");
                }

                sh_ptr<Object> object(new Object());

                std::map<std::string, sh_ptr<Value> > activationRecord;

                args.insert(args.begin(), object);

                for (std::size_t i = 0; i < constructor_fn->params.size(); i++)
                {
                    std::string param = std::dynamic_pointer_cast<IdentifierAST>(constructor_fn->params.at(i))->name;
                    activationRecord[param] = args.at(i);
                }

                sh_ptr<Environment> activationEnv(new Environment(activationRecord, constructor_fn->env));

                std::vector<sh_ptr<AST> > body = std::dynamic_pointer_cast<BlockStatementAST>(constructor_fn->body)->body;

                this->eval(body, activationEnv, true);

                object->members = activationEnv;

                return object;
            }

            // 3. User-defined function:
            if (userDefined)
            {
                std::map<std::string, sh_ptr<Value> > activationRecord;

                for (std::size_t i = 0; i < userDefined->params.size(); i++)
                {
                    std::string param = std::dynamic_pointer_cast<IdentifierAST>(userDefined->params.at(i))->name;
                    activationRecord[param] = args.at(i);
                }

                sh_ptr<Environment> activationEnv(new Environment(activationRecord, userDefined->env));

                std::vector<sh_ptr<AST> > body = std::dynamic_pointer_cast<BlockStatementAST>(userDefined->body)->body;

                sh_ptr<Value> result = this->eval(body, activationEnv, true);

                if (!result)
                {
                    return sh_ptr<Null>(new Null());
                }

                return result;
            }

            this->error(exp->callee->line, callee->str() + " is not callable");
            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(MemberExpressionAST *exp, sh_ptr<Environment> env)
        {
            if (exp->subtype == "computed")
            {
                return sh_ptr<Value>();
            }

            sh_ptr<Value> value = this->eval(exp->object, env);

            sh_ptr<Object> object = std::dynamic_pointer_cast<Object>(value);

            if (!object)
            {
                error(exp->object->line, value->str() + " does not support member access");
            }

            std::string property = std::dynamic_pointer_cast<IdentifierAST>(exp->property)->name;

            try
            {
                return object->members->lookup(property);
            }
            catch (const std::string &e)
            {
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

        sh_ptr<Value> eval(ReturnStatementAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Value> argument(new Null());

            if (exp->argument)
            {
                argument = this->eval(exp->argument, env);
            }

            this->returnValue = argument;

            return argument;
        }

        sh_ptr<Value> eval(AssignmentExpressionAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Value> right = this->eval(exp->right, env);

            if (exp->left->type() == "MemberExpression")
            {
                sh_ptr<MemberExpressionAST> left = std::dynamic_pointer_cast<MemberExpressionAST>(exp->left);
                sh_ptr<Value> value = eval(left->object, env);
                sh_ptr<Object> object = std::dynamic_pointer_cast<Object>(value);

                std::string property = std::dynamic_pointer_cast<IdentifierAST>(left->property)->name;

                if (!object)
                {
                    error(left->object->line, value->str() + " does not support member assignment");

                    return sh_ptr<Value>();
                }

                if (exp->op == "=")
                {
                    object->members->define(property, right);

                    return right;
                }

                sh_ptr<Number> number = std::dynamic_pointer_cast<Number>(right);

                sh_ptr<Number> currentValue;

                try
                {
                    currentValue = std::dynamic_pointer_cast<Number>(object->members->lookup(property));
                }
                catch (const std::string &e)
                {
                    // Error messages thrown from outside of interpreter don't have a line number
                    // so insert one here

                    error(exp->line, e);
                    return right;
                }

                if (!currentValue)
                {
                    error(exp->right->line, "invalid argument #1 for binary operator \"" + exp->op + "\"");

                    return sh_ptr<Value>();
                }

                if (!number)
                {
                    error(exp->right->line, "invalid argument #2 for binary operator \"" + exp->op + "\"");

                    return sh_ptr<Value>();
                }

                if (currentValue->isInt() && number->isInt())
                {
                    if (exp->op == "+=")
                    {
                        object->members->assign(property, sh_ptr<Int>(new Int(currentValue->intVal() + number->intVal())));

                        return right;
                    }

                    if (exp->op == "-=")
                    {
                        object->members->assign(property, sh_ptr<Int>(new Int(currentValue->intVal() - number->intVal())));

                        return right;
                    }

                    if (exp->op == "*=")
                    {
                        object->members->assign(property, sh_ptr<Int>(new Int(currentValue->intVal() * number->intVal())));

                        return right;
                    }

                    if (exp->op == "/=")
                    {
                        if(number->intVal() == 0)
                        {
                            error(exp->right->line, "integer division by zero");

                            return sh_ptr<Value>();
                        }

                        object->members->assign(property, sh_ptr<Int>(new Int(currentValue->intVal() / number->intVal())));

                        return right;
                    }
                }
                else
                {

                    if (exp->op == "+=")
                    {
                        object->members->assign(property, sh_ptr<Double>(new Double(currentValue->doubleVal() + number->doubleVal())));

                        return right;
                    }

                    if (exp->op == "-=")
                    {
                        object->members->assign(property, sh_ptr<Double>(new Double(currentValue->doubleVal() - number->doubleVal())));

                        return right;
                    }

                    if (exp->op == "*=")
                    {
                        object->members->assign(property, sh_ptr<Double>(new Double(currentValue->doubleVal() * number->doubleVal())));

                        return right;
                    }

                    if (exp->op == "/=")
                    {
                        if(number->doubleVal() == 0.0)
                        {
                            object->members->assign(property, Double::inf);

                            return right;
                        }
                    
                        object->members->assign(property, sh_ptr<Double>(new Double(currentValue->doubleVal() / number->doubleVal())));

                        return right;
                    }
                }

                return right;
            }

            std::string name = std::dynamic_pointer_cast<IdentifierAST>(exp->left)->name;

            if (exp->op == "=")
            {
                env->assign(name, right);

                return right;
            }

            sh_ptr<Number> number = std::dynamic_pointer_cast<Number>(right);

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
                return right;
            }

            if (!currentValue)
            {
                error(exp->right->line, "invalid argument #1 for binary operator \"" + exp->op + "\"");

                return sh_ptr<Value>();
            }

            if (!number)
            {
                error(exp->right->line, "invalid argument #2 for binary operator \"" + exp->op + "\"");

                return sh_ptr<Value>();
            }

            if (currentValue->isInt() && number->isInt())
            {
                if (exp->op == "+=")
                {
                    env->assign(name, sh_ptr<Int>(new Int(currentValue->intVal() + number->intVal())));

                    return right;
                }

                if (exp->op == "-=")
                {
                    env->assign(name, sh_ptr<Int>(new Int(currentValue->intVal() - number->intVal())));

                    return right;
                }

                if (exp->op == "*=")
                {
                    env->assign(name, sh_ptr<Int>(new Int(currentValue->intVal() * number->intVal())));

                    return right;
                }

                if (exp->op == "/=")
                {

                    if(number->intVal() == 0)
                    {
                        error(exp->right->line, "integer division by zero");

                        return sh_ptr<Value>();
                    }

                    env->assign(name, sh_ptr<Int>(new Int(currentValue->intVal() / number->intVal())));

                    return right;
                }
            }
            else
            {
                if (exp->op == "+=")
                {
                    env->assign(name, sh_ptr<Double>(new Double(currentValue->doubleVal() + number->doubleVal())));

                    return right;
                }

                if (exp->op == "-=")
                {
                    env->assign(name, sh_ptr<Double>(new Double(currentValue->doubleVal() - number->doubleVal())));

                    return right;
                }

                if (exp->op == "*=")
                {
                    env->assign(name, sh_ptr<Double>(new Double(currentValue->doubleVal() * number->doubleVal())));

                    return right;
                }

                if (exp->op == "/=")
                {
                    if(number->doubleVal() == 0.0)
                    {
                        env->assign(name, Double::inf);

                        return right;
                    }

                    env->assign(name, sh_ptr<Double>(new Double(currentValue->doubleVal() / number->doubleVal())));

                    return right;
                }
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
            sh_ptr<Value> leftValue = this->eval(exp->left, env);
            sh_ptr<Value> rightValue = this->eval(exp->right, env);

            if (exp->op == "==")
            {
                return sh_ptr<Boolean>(new Boolean(leftValue->eq(rightValue)));
            }

            if (exp->op == "!=")
            {
                return sh_ptr<Boolean>(new Boolean(!(leftValue->eq(rightValue))));
            }

            sh_ptr<Number> left = std::dynamic_pointer_cast<Number>(left);
            sh_ptr<Number> right = std::dynamic_pointer_cast<Number>(right);

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

            if (left->isInt() && right->isInt())
            {
                if (exp->op == "+")
                {
                    return sh_ptr<Int>(new Int(left->intVal() + right->intVal()));
                }

                if (exp->op == "-")
                {
                    return sh_ptr<Int>(new Int(left->intVal() - right->intVal()));
                }

                if (exp->op == "*")
                {
                    return sh_ptr<Int>(new Int(left->intVal() * right->intVal()));
                }

                if (exp->op == "/")
                {
                    if(right->intVal() == 0)
                    {
                        error(exp->right->line, "integer division by zero");

                        return sh_ptr<Value>();
                    }

                    return sh_ptr<Int>(new Int(left->intVal() / right->intVal()));
                }

                if (exp->op == ">")
                {
                    return sh_ptr<Boolean>(new Boolean(left->intVal() > right->intVal()));
                }

                if (exp->op == ">=")
                {
                    return sh_ptr<Boolean>(new Boolean(left->intVal() >= right->intVal()));
                }

                if (exp->op == "<")
                {
                    return sh_ptr<Boolean>(new Boolean(left->intVal() < right->intVal()));
                }

                if (exp->op == "<=")
                {
                    return sh_ptr<Boolean>(new Boolean(left->intVal() <= right->intVal()));
                }

                return sh_ptr<Value>();
            }
            else
            {

                if (exp->op == "+")
                {
                    return sh_ptr<Double>(new Double(left->doubleVal() + right->doubleVal()));
                }

                if (exp->op == "-")
                {
                    return sh_ptr<Double>(new Double(left->doubleVal() - right->doubleVal()));
                }

                if (exp->op == "*")
                {
                    return sh_ptr<Double>(new Double(left->doubleVal() * right->doubleVal()));
                }

                if (exp->op == "/")
                {
                    if(right->doubleVal() == 0.0)
                    {
                        return Double::inf;
                    }

                    return sh_ptr<Double>(new Double(left->doubleVal() / right->doubleVal()));
                }

                if (exp->op == ">")
                {
                    return sh_ptr<Boolean>(new Boolean(left->doubleVal() > right->doubleVal()));
                }

                if (exp->op == ">=")
                {
                    return sh_ptr<Boolean>(new Boolean(left->doubleVal() >= right->doubleVal()));
                }

                if (exp->op == "<")
                {
                    return sh_ptr<Boolean>(new Boolean(left->doubleVal() < right->doubleVal()));
                }

                if (exp->op == "<=")
                {
                    return sh_ptr<Boolean>(new Boolean(left->doubleVal() <= right->doubleVal()));
                }
            }

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(IntLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<Int>(new Int(exp->value));
        }

        sh_ptr<Value> eval(DoubleLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<Double>(new Double(exp->value));
        }

        sh_ptr<Value> eval(StringLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<String>(new String(exp->string, sh_ptr<Environment>(new Environment({}, builtins::String->members))));
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