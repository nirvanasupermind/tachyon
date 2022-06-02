#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "values.hpp"

namespace eris
{
    namespace builtins
    {
        sh_ptr<Class> Object{new Class(sh_ptr<Environment>(new Environment()))};

        sh_ptr<Class> String{new Class(sh_ptr<Environment>(new Environment({}, Object->members)))};

        sh_ptr<NativeFunction> Stringlen{new NativeFunction([](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<eris::String> self = std::dynamic_pointer_cast<eris::String>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"length\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            return sh_ptr<Int>(new Int(self->string.length())); })};

        sh_ptr<NativeFunction> print{new NativeFunction([](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                        { 
                                                            sh_ptr<Value> data = args.at(0);

                                                            std::cout << data->str() << '\n';

                                                            return sh_ptr<Null>(new Null()); })};
    };
}

#endif