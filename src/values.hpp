#ifndef VALUES_HPP
#define VALUES_HPP

#include "aliases.hpp"

namespace eris
{
    class Value
    {
    public:
        virtual ~Value() = default;
        virtual std::string str() const = 0;
        virtual bool truthy()
        {
            return true;
        }
    };

    class Null: public Value
    {
    public:
        std::string str() const 
        {
            return "null";
        }

        bool truthy()
        {
            return false;
        }
    };

    class Number : public Value
    {
    public:
        double value;

        Number(double value) : value(value) {}

        std::string str() const
        {
            return std::to_string(value);
        }
    };

    class Boolean : public Value
    {
    public:
        bool value;

        Boolean(bool value) : value(value) {}

        bool truthy()
        {
            return value;
        }

        std::string str() const
        {
            return value ? "true" : "false";
        }
    };

    class Environment: public std::enable_shared_from_this<Environment>
    {
    public:
        std::map<std::string, sh_ptr<Value> > record;
        sh_ptr<Environment> parent;

        Environment(std::map<std::string, sh_ptr<Value> > record = {},
                    sh_ptr<Environment> parent = sh_ptr<Environment>())
            : record(record), parent(parent)
        {
        }

        /**
         * Creates a variable with the given name and value.
         */
        void define(const std::string &name, sh_ptr<Value> value)
        {
            this->record[name] = value;
        }

        /**
         * Updates an existing variable.
         */
        void assign(const std::string &name, sh_ptr<Value> value)
        {
            this->resolve(name)->record[name] = value;
        }

        /**
         * Returns the value of a defined variable, or null pointer
         * if the variable is not defined.
         */
        sh_ptr<Value> lookup(const std::string &name)
        {
            return this->resolve(name)->record.at(name);
        }

        /**
         * Returns the value of a defined variable, or null pointer
         * if the variable is not defined.
         */
        sh_ptr<Environment> resolve(const std::string &name)
        {
            if(this->record.count(name) == 1)
            {
                return shared_from_this();
            }

            if(this->parent)
            {
                return this->parent;
            }
                    
            throw std::string("variable \""+name+"\" is not defined");
            return sh_ptr<Environment>();
        }
    };
    
    class Object : public Value
    {
    public:
        sh_ptr<Environment> env;

        Object() = default;

        Object(sh_ptr<Environment> env) : env(env) {}

        std::string str() const
        {
            return "<object>";
        }
    };

    class String : public Object
    {
    public:
        std::string string;

        String(const std::string &string) : string(string) {}

        std::string str() const
        {
            return string;
        }
    };
}

#endif