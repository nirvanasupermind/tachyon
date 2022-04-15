#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "values.hpp"
#include "aliases.hpp"

namespace eris
{
    class Environment
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
    };
}

#endif