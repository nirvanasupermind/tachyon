#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include "values.hpp"

namespace eris
{
    namespace builtins
    {
        sh_ptr<Class> Object{new Class(sh_ptr<Environment>(new Environment()))};

        sh_ptr<NativeFunction> Object_constructor{new NativeFunction(1, "constructor", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            {
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class Object> self = std::dynamic_pointer_cast<class Object>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"constructor\"");
                                                                return sh_ptr<Value>();
                                                            } 
                                                            return sh_ptr<Null>(new Null()); })};

        sh_ptr<Class> String{new Class(sh_ptr<Environment>(new Environment({}, Object->members)))};

        sh_ptr<NativeFunction> String_at{new NativeFunction(2, "at", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class String> self = std::dynamic_pointer_cast<class String>(selfValue);

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
                                                                sh_ptr<Environment> members(new Environment({}, String->members));

                                                                if(idx->value < 0)
                                                                {
                                                                    return sh_ptr<class String>(new (class String)(std::string(1, self->string.at(self->string.size() + idx->value)), members));                                                                     
                                                                }
                                                                
                                                                return sh_ptr<class String>(new (class String)(std::string(1, self->string.at(idx->value)), members)); 
                                                            }
                                                            catch(const std::out_of_range &e)
                                                            {
                                                                throw std::string("string index out of range");
                                                                return sh_ptr<Value>();
                                                            } })};

        sh_ptr<NativeFunction> String_concat{new NativeFunction(2, "concat", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<Value> otherValue = args.at(1);

                                                            sh_ptr<class String> self = std::dynamic_pointer_cast<class String>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"concat\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            sh_ptr<class String> other = std::dynamic_pointer_cast<class String>(otherValue);

                                                            if(!other)
                                                            {
                                                                throw std::string("invalid argument #2 for function \"concat\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            sh_ptr<Environment> members(new Environment({}, String->members));

                                                            return sh_ptr<class String>(new (class String)(self->string + other->string, members)); })};

        sh_ptr<NativeFunction> String_find{new NativeFunction(2, "find", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<Value> searchValue = args.at(1);

                                                            sh_ptr<class String> self = std::dynamic_pointer_cast<class String>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"find\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            sh_ptr<class String> searchStr = std::dynamic_pointer_cast<class String>(searchValue);

                                                            if(!searchStr)
                                                            {
                                                                throw std::string("invalid argument #2 for function \"find\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            std::size_t result = self->string.find_first_of(searchStr->string);

                                                            if(result == std::string::npos)
                                                            {
                                                                return sh_ptr<Null>(new Null());
                                                            }

                                                            return sh_ptr<Int>(new Int(result)); })};

        sh_ptr<NativeFunction> String_findLast{new NativeFunction(2, "findLast", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<Value> searchValue = args.at(1);

                                                            sh_ptr<class String> self = std::dynamic_pointer_cast<class String>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"find\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            sh_ptr<class String> searchStr = std::dynamic_pointer_cast<class String>(searchValue);

                                                            if(!searchStr)
                                                            {
                                                                throw std::string("invalid argument #2 for function \"find\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            std::size_t result = self->string.find_last_of(searchStr->string);

                                                            if(result == std::string::npos)
                                                            {
                                                                return sh_ptr<Null>(new Null());
                                                            }

                                                            return sh_ptr<Int>(new Int(result)); })};
    
        sh_ptr<NativeFunction> String_len{new NativeFunction(1, "len", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class String> self = std::dynamic_pointer_cast<class String>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"len\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            return sh_ptr<Int>(new Int(self->string.size())); })};


        sh_ptr<Class> List{new Class(sh_ptr<Environment>(new Environment({}, Object->members)))};

        sh_ptr<NativeFunction> List_add{new NativeFunction(2, "add", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class List> self = std::dynamic_pointer_cast<class List>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"at\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            self->vec.push_back(args.at(1));
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> List_del{new NativeFunction(2, "del", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class List> self = std::dynamic_pointer_cast<class List>(selfValue);

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
                                                                std::vector<sh_ptr<Value> >::iterator q = self->vec.begin();

                                                                if(idx->value < 0)
                                                                {
                                                                    q += self->vec.size() + idx->value; 
                                                                } 
                                                                else
                                                                {
                                                                    q += idx->value; 
                                                                }
                                                                
                                                                sh_ptr<Value> result = self->vec.at(idx->value);

                                                                self->vec.erase(q);

                                                                return result;
                                                            }

                                                            catch(const std::out_of_range &e)
                                                            {
                                                                throw std::string("list index out of range");
                                                                return sh_ptr<Value>();
                                                            } 

                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> List_at{new NativeFunction(1, "at", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class List> self = std::dynamic_pointer_cast<class List>(selfValue);

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
                                                                    return self->vec.at(self->vec.size() + idx->value);     
                                                                }
                                                                
                                                                return self->vec.at(idx->value);
                                                            }

                                                            catch(const std::out_of_range &e)
                                                            {
                                                                throw std::string("list index out of range");
                                                                return sh_ptr<Value>();
                                                            } })};

        sh_ptr<NativeFunction> List_atPut{new NativeFunction(3, "atPut", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class List> self = std::dynamic_pointer_cast<class List>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"atPut\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            sh_ptr<Value> idxValue = args.at(1);

                                                            sh_ptr<Int> idx = std::dynamic_pointer_cast<Int>(idxValue);

                                                            if(!idx)
                                                            {
                                                                throw std::string("invalid argument #2 for function \"atPut\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            sh_ptr<Value> value = args.at(2);

                                                            if(idx->value == self->vec.size())
                                                            {
                                                                self->vec.push_back(value);

                                                                return value;
                                                            }
                                                        
                                                            if(idx->value > self->vec.size() || idx->value < -self->vec.size())
                                                            {
                                                                throw std::string("list index out of range");
                                                                return sh_ptr<Value>();   
                                                            }
                                                            
                                                            try
                                                            {
                                                                if(idx->value < 0)
                                                                {
                                                                    self->vec[self->vec.size() + idx->value] = args.at(3);     
                                                                }
                                                                
                                                                self->vec[idx->value] = value;

                                                                return args.at(2);
                                                            }

                                                            catch(const std::out_of_range &e)
                                                            {
                                                                throw std::string("list index out of range");
                                                                return sh_ptr<Value>();
                                                            } })};

        sh_ptr<NativeFunction> List_len{new NativeFunction(1, "len", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class List> self = std::dynamic_pointer_cast<class List>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"len\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            return sh_ptr<Int>(new Int(self->vec.size())); })};
        sh_ptr<NativeFunction> print{new NativeFunction(1, "print", [](std::vector<sh_ptr<Value> > args) -> sh_ptr<Value>
                                                        {                                                             
                                                            sh_ptr<Value> data = args.at(0);

                                                            std::cout << data->str() << '\n';

                                                            return sh_ptr<Null>(new Null()); })};
                                                  
    };
}

#endif