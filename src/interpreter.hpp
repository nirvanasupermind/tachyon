#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include "parser.hpp"
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

            if (type == "CharLiteral")
            {
                return this->eval(dynamic_cast<CharLiteralAST *>(exp), env);
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

            if (type == "ListLiteral")
            {
                return this->eval(dynamic_cast<ListLiteralAST *>(exp), env);
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

            if (type == "UnaryExpression")
            {
                return this->eval(dynamic_cast<UnaryExpressionAST *>(exp), env);
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

            if (type == "RequireStatement")
            {
                return this->eval(dynamic_cast<RequireStatementAST *>(exp), env);
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

    protected:
        sh_ptr<Value> eval(NamespaceDeclarationAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Environment> namespaceEnv(new Environment({}, env));

            if(env->contains(exp->name))
            {
                sh_ptr<Namespace> ns = std::dynamic_pointer_cast<Namespace>(env->lookup(exp->name));
                if(ns)
                {
                    namespaceEnv = ns->members;
                }
            }

            std::vector<sh_ptr<AST> > body = std::dynamic_pointer_cast<BlockStatementAST>(exp->body)->body;

            this->eval(body, namespaceEnv);

            env->define(exp->name, sh_ptr<Namespace>(new Namespace(namespaceEnv)));

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(ClassDeclarationAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Environment> declarationEnv(new Environment({}, env));

            std::vector<sh_ptr<AST> > body = std::dynamic_pointer_cast<BlockStatementAST>(exp->body)->body;
            this->eval(body, declarationEnv);

            sh_ptr<Environment> classEnv(new Environment(declarationEnv->record));

            if (exp->superClass)
            {
                sh_ptr<Value> superValue = this->eval(exp->superClass, env);
                sh_ptr<Class> superClass = std::dynamic_pointer_cast<Class>(superValue);


                if (!superClass)
                {
                    this->error(exp->superClass->line, superValue->str() + " cannot be used as a superclass");
                    return sh_ptr<Value>();
                }

                classEnv->parent = superClass->members;
            }
            else
            {
                classEnv->parent = builtins::Object->members;
            }

            env->define(exp->name, sh_ptr<Class>(new Class(classEnv)));
            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(FunctionDeclarationAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<UserDefinedFunction> fn = sh_ptr<UserDefinedFunction>(new UserDefinedFunction(exp->name, exp->params, exp->body, env));
            env->define(exp->name, fn);

            return sh_ptr<Value>();
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

        sh_ptr<Value> eval(RequireStatementAST *exp, sh_ptr<Environment> env)
        {
            std::string filename = exp->path;

            std::ifstream inFile;

            inFile.open(filename);

            std::stringstream strStream;
            strStream << inFile.rdbuf();

            std::string text = strStream.str();

            if(text == "") {
                this->error(exp->line, "File \"" + filename + "\" is empty or does not exist"); 
                return sh_ptr<Value>();
            }

            Parser parser;

            this->eval(parser.parse(text), global);
                
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
                    error(exp->left->line, "invalid argument #1 for binary operator \"" + exp->op + "\"");

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

            sh_ptr<Number> leftNum = std::dynamic_pointer_cast<Number>(leftValue);
            sh_ptr<Number> rightNum = std::dynamic_pointer_cast<Number>(rightValue);

            if (!leftNum)
            {
                error(exp->left->line, "invalid argument #1 for binary operator \"" + exp->op + "\"");
                return sh_ptr<Value>();
            }

            if (!rightNum)
            {
                error(exp->right->line, "invalid argument #2 for binary operator \"" + exp->op + "\"");
                return sh_ptr<Value>();
            }

            if (leftNum->isInt() && rightNum->isInt())
            {
                if (exp->op == "+")
                {
                    return sh_ptr<Int>(new Int(leftNum->intVal() + rightNum->intVal()));
                }

                if (exp->op == "-")
                {
                    return sh_ptr<Int>(new Int(leftNum->intVal() - rightNum->intVal()));
                }

                if (exp->op == "*")
                {
                    return sh_ptr<Int>(new Int(leftNum->intVal() * rightNum->intVal()));
                }

                if (exp->op == "/")
                {
                    if(rightNum->intVal() == 0)
                    {
                        error(exp->right->line, "integer division by zero");

                        return sh_ptr<Value>();
                    }

                    return sh_ptr<Int>(new Int(leftNum->intVal() / rightNum->intVal()));
                }

                if (exp->op == ">")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->intVal() > rightNum->intVal()));
                }

                if (exp->op == ">=")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->intVal() >= rightNum->intVal()));
                }

                if (exp->op == "<")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->intVal() < rightNum->intVal()));
                }

                if (exp->op == "<=")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->intVal() <= rightNum->intVal()));
                }

                return sh_ptr<Value>();
            }
            else
            {

                if (exp->op == "+")
                {
                    return sh_ptr<Double>(new Double(leftNum->doubleVal() + rightNum->doubleVal()));
                }

                if (exp->op == "-")
                {
                    return sh_ptr<Double>(new Double(leftNum->doubleVal() - rightNum->doubleVal()));
                }

                if (exp->op == "*")
                {
                    return sh_ptr<Double>(new Double(leftNum->doubleVal() * rightNum->doubleVal()));
                }

                if (exp->op == "/")
                {
                    if(rightNum->doubleVal() == 0.0)
                    {
                        return Double::inf;
                    }

                    return sh_ptr<Double>(new Double(leftNum->doubleVal() / rightNum->doubleVal()));
                }

                if (exp->op == ">")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->doubleVal() > rightNum->doubleVal()));
                }

                if (exp->op == ">=")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->doubleVal() >= rightNum->doubleVal()));
                }

                if (exp->op == "<")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->doubleVal() < rightNum->doubleVal()));
                }

                if (exp->op == "<=")
                {
                    return sh_ptr<Boolean>(new Boolean(leftNum->doubleVal() <= rightNum->doubleVal()));
                }
            }

            return sh_ptr<Value>();
        }

        sh_ptr<Value> eval(UnaryExpressionAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Value> argValue = this->eval(exp->argument, env);

            sh_ptr<Number> argNum = std::dynamic_pointer_cast<Number>(argValue);
            
            sh_ptr<Boolean> argBool = std::dynamic_pointer_cast<Boolean>(argValue);

            if(argNum)
            {
                if(exp->op == "+")
                {
                    return argNum;
                }


                if(exp->op == "-")
                {
                    if(argNum->isInt())
                    {
                        return sh_ptr<Int>(new Int(-argNum->intVal()));
                    }

                    return sh_ptr<Double>(new Double(-argNum->doubleVal()));
                }
                
            }

            if(argBool)
            {
                return sh_ptr<Boolean>(new Boolean(!argBool->value));           
            }
            
            error(exp->argument->line, "invalid argument #1 for unary operator \"" + exp->op + "\"");
            return sh_ptr<Value>();
        }


        sh_ptr<Value> call(sh_ptr<Value> callee, std::vector<sh_ptr<Value> > &args)
        {
            sh_ptr<NativeFunction> native = std::dynamic_pointer_cast<NativeFunction>(callee);

            sh_ptr<Class> cls = std::dynamic_pointer_cast<Class>(callee);

            sh_ptr<UserDefinedFunction> userDefined = std::dynamic_pointer_cast<UserDefinedFunction>(callee);
      
            // 1. Native function:
            if (native)
            {
                if(args.size() < native->arity)
                {
                    throw std::string("missing argument #"+std::to_string(args.size() + 1) + " for function \"" + native->name+"\"");
                }

                return native->fn(args);
            }

            // 2. Class instantiation:
            if (cls)
            {
                sh_ptr<Value> constructor = cls->members->lookup("constructor");

                sh_ptr<Object> object(new Object(sh_ptr<Environment>(new Environment({}, cls->members))));

                args.insert(args.begin(), object);

                call(constructor, args);

                return object;
            }

            // 3. User-defined function:
            if (userDefined)
            {
                if(args.size() < userDefined->arity)
                {
                    throw std::string("missing argument #"+std::to_string(args.size() + 1) + " for function \"" + userDefined->name+"\"");
                }

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

            throw std::string(callee->str() + " is not callable");
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

            for(int i = 0; i < exp->arguments.size(); i++)
            {
                args.push_back(this->eval(exp->arguments.at(i), env));
            }

            // std::cout << args.size() << '\n';

            try
            {
                return call(callee, args);
            }
            catch(const std::string &e)
            {
                throw std::string(e);
                return sh_ptr<Value>();
            }
        }

        sh_ptr<Value> eval(MemberExpressionAST *exp, sh_ptr<Environment> env)
        {
            sh_ptr<Value> value = this->eval(exp->object, env);

            sh_ptr<Class> cls = std::dynamic_pointer_cast<Class>(value);

            sh_ptr<Namespace> ns = std::dynamic_pointer_cast<Namespace>(value);

            sh_ptr<Object> object = std::dynamic_pointer_cast<Object>(value);

            std::string property = std::dynamic_pointer_cast<IdentifierAST>(exp->property)->name;

            if(cls)
            {
                try
                {
                    return cls->members->lookup(property);
                }
                catch (const std::string &e)
                {
                    // Error messages thrown from outside of interpreter don't have a line number
                    // so insert one here

                    error(exp->line, e);
                    return sh_ptr<Value>();
                }
            }
            
            if(ns)
            {
                try
                {
                    return ns->members->lookup(property);
                }
                catch (const std::string &e)
                {
                    // Error messages thrown from outside of interpreter don't have a line number
                    // so insert one here

                    error(exp->line, e);
                    return sh_ptr<Value>();
                }
            }

            if(object)
            {
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

            error(exp->object->line, value->str() + " does not support member access");
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

        sh_ptr<Value> eval(CharLiteralAST *exp, sh_ptr<Environment> env)
        {
            return sh_ptr<Char>(new Char(exp->value));
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
            return sh_ptr<Null>(new Null());
        }

        sh_ptr<Value> eval(ListLiteralAST *exp, sh_ptr<Environment> env)
        {
            std::vector<sh_ptr<Value> > elements;

            for(sh_ptr<AST> elem : exp->elements)
            {  
                elements.push_back(this->eval(elem, env));
            }

            sh_ptr<Environment> members(new Environment({}, builtins::List->members));

            return sh_ptr<List>(new List(elements, members));
        }
    };
}

#endif