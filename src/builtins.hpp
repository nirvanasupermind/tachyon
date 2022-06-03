#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "values.hpp"

namespace eris
{
    namespace builtins
    {
        sh_ptr<Class> Object{new Class(sh_ptr<Environment>(new Environment()))};

        sh_ptr<Class> String{new Class(sh_ptr<Environment>(new Environment({}, Object->members)))};

        sh_ptr<NativeFunction> Stringat{new NativeFunction([](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<eris::String> self = std::dynamic_pointer_cast<class String>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"at\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            sh_ptr<Value> idxValue = args.at(1);

                                                            sh_ptr<Int> idx = std::dynamic_pointer_cast<Int>(idxValue);

                                                            if(!idx)
                                                            {
                                                                throw std::string("invalid argument #2 for function \"at\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                        
                                                            try
                                                            {
                                                                if(idx->value < 0)
                                                                {
                                                                    return sh_ptr<class String>(new (class String)(std::string(1, self->string.at(self->string.size() + idx->value)), String->members));                                                                     
                                                                }
                                                                return sh_ptr<class String>(new (class String)(std::string(1, self->string.at(idx->value)), String->members)); 
                                                            }
                                                            catch(const std::out_of_range &e)
                                                            {
                                                                throw std::string("string index out of range");
                                                                return sh_ptr<Value>();
                                                            } })};

        sh_ptr<NativeFunction> Stringlen{new NativeFunction([](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<eris::String> self = std::dynamic_pointer_cast<eris::String>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"len\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            return sh_ptr<Int>(new Int(self->string.size())); })};

        sh_ptr<NativeFunction> print{new NativeFunction([](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                        { 
                                                            sh_ptr<Value> data = args.at(0);

                                                            std::cout << data->str() << '\n';

                                                            return sh_ptr<Null>(new Null()); })};
    };
}

#endif