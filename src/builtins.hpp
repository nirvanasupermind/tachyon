#ifndef BUILTINS_HPP
#define BUILTINS_HPP

namespace eris
{
    namespace builtins
    {
        sh_ptr<Class> Object {new Class(sh_ptr<Environment>(new Environment())) };

        sh_ptr<NativeFunction> print { new NativeFunction([](std::vector<sh_ptr<Value> > args)
                                                        { 
                                                            sh_ptr<Value> data = args.at(0);

                                                            std::cout << data->str() << '\n';

                                                            return sh_ptr<Null>(new Null()); }) };
    };
}

#endif