#ifndef BUILTINS_HPP
#define BUILTINS_HPP

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <cmath>

#include "values.hpp"
#include "random.hpp"

namespace eris
{
    namespace builtins
    {
        extern sh_ptr<Class> Function;

        sh_ptr<Class> Object{new Class(sh_ptr<Environment>(new Environment()))};

        sh_ptr<NativeFunction> Object_constructor{new NativeFunction(1, "constructor", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> String_at{new NativeFunction(2, "at", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> String_concat{new NativeFunction(2, "concat", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> String_find{new NativeFunction(2, "find", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> String_findLast{new NativeFunction(2, "findLast", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> String_len{new NativeFunction(1, "len", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> List_add{new NativeFunction(2, "add", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> List_at{new NativeFunction(1, "at", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> List_atPut{new NativeFunction(3, "atPut", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> List_del{new NativeFunction(2, "del", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
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

        sh_ptr<NativeFunction> List_len{new NativeFunction(1, "len", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> selfValue = args.at(0);

                                                            sh_ptr<class List> self = std::dynamic_pointer_cast<class List>(selfValue);

                                                            if(!self)
                                                            {
                                                                throw std::string("invalid argument #1 for function \"len\"");
                                                                return sh_ptr<Value>();
                                                            }
                                                            
                                                            return sh_ptr<Int>(new Int(self->vec.size())); })};

        sh_ptr<Class> Function{new Class(sh_ptr<Environment>(new Environment({}, Object->members)))};

        sh_ptr<Class> Math{new Class(sh_ptr<Environment>(new Environment({}, Object->members)))};

        sh_ptr<Double> Math_PI{new Double(3.141592653589793)};

        sh_ptr<Double> Math_E{new Double(2.718281828459045)};

        sh_ptr<NativeFunction> Math_abs{new NativeFunction(1, "abs", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Int> intVal = std::dynamic_pointer_cast<Int>(value);

                                                            if (intVal) 
                                                            {
                                                                return sh_ptr<Int>(new Int(std::abs(intVal->value)));
                                                            }

                                                            sh_ptr<Double> doubleVal = std::dynamic_pointer_cast<Double>(value);

                                                            if (doubleVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::abs(doubleVal->value)));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"abs\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_acos{new NativeFunction(1, "acos", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Int> intVal = std::dynamic_pointer_cast<Int>(value);

                                                            if (intVal) 
                                                            {
                                                                return sh_ptr<Int>(new Int(std::acos(intVal->value)));
                                                            }

                                                            sh_ptr<Double> doubleVal = std::dynamic_pointer_cast<Double>(value);

                                                            if (doubleVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::acos(doubleVal->value)));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"acos\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_asin{new NativeFunction(1, "asin", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::asin(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"asin\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_atan{new NativeFunction(1, "atan", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                            { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::atan(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"atan\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_ceil{new NativeFunction(1, "ceil", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                            {
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Int> intVal = std::dynamic_pointer_cast<Int>(value);

                                                            if (intVal) 
                                                            {
                                                                return sh_ptr<Int>(new Int(std::ceil(intVal->value)));
                                                            }

                                                            sh_ptr<Double> doubleVal = std::dynamic_pointer_cast<Double>(value);

                                                            if (doubleVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::ceil(doubleVal->value)));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"ceil\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_cos{new NativeFunction(1, "cos", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::cos(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"cos\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_exp{new NativeFunction(1, "exp", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::exp(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"exp\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_floor{new NativeFunction(1, "floor", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                             { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::floor(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"exp\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_log{new NativeFunction(1, "log", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::log(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"log\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_max{new NativeFunction(2, "max", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Number> numA = std::dynamic_pointer_cast<Number>(args.at(0));

                                                            if (!numA) 
                                                            {
                                                                throw std::string("invalid argument #1 for function \"max\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            sh_ptr<Number> numB = std::dynamic_pointer_cast<Number>(args.at(1));

                                                            if (!numB) 
                                                            {
                                                                throw std::string("invalid argument #2 for function \"max\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            if (numA->isInt() && numB->isInt()) {
                                                                return sh_ptr<Int>(new Int(std::max(numA->intVal(), numB->intVal())));
                                                            }

                                                            return sh_ptr<Double>(new Double(std::max(numA->doubleVal(), numB->doubleVal()))); })};

        sh_ptr<NativeFunction> Math_min{new NativeFunction(2, "min", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Number> numA = std::dynamic_pointer_cast<Number>(args.at(0));

                                                            if (!numA) 
                                                            {
                                                                throw std::string("invalid argument #1 for function \"min\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            sh_ptr<Number> numB = std::dynamic_pointer_cast<Number>(args.at(1));

                                                            if (!numB) 
                                                            {
                                                                throw std::string("invalid argument #2 for function \"min\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            if (numA->isInt() && numB->isInt()) 
                                                            {
                                                                return sh_ptr<Int>(new Int(std::min(numA->intVal(), numB->intVal())));
                                                            }

                                                            return sh_ptr<Double>(new Double(std::min(numA->doubleVal(), numB->doubleVal()))); })};

        sh_ptr<NativeFunction> Math_pow{new NativeFunction(2, "pow", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Number> numA = std::dynamic_pointer_cast<Number>(args.at(0));

                                                            if (!numA) 
                                                            {
                                                                throw std::string("invalid argument #1 for function \"pow\"");
                                                                return sh_ptr<Value>();
                                                            }


                                                            sh_ptr<Number> numB = std::dynamic_pointer_cast<Number>(args.at(1));

                                                            if (!numB) 
                                                            {
                                                                throw std::string("invalid argument #2 for function \"pow\"");
                                                                return sh_ptr<Value>();
                                                            }

                                                            return sh_ptr<Double>(new Double(std::pow(numA->doubleVal(), numB->doubleVal()))); })};

        sh_ptr<NativeFunction> Math_rand{new NativeFunction(0, "rand", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                            { return sh_ptr<Double>(new Double(dis(gen))); })};

        sh_ptr<NativeFunction> Math_round{new NativeFunction(1, "round", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                             { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::round(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"round\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_sin{new NativeFunction(1, "sin", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::sin(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"sin\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> Math_tan{new NativeFunction(1, "tan", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           { 
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Number> numVal = std::dynamic_pointer_cast<Number>(value);

                                                            if (numVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(std::tan(numVal->doubleVal())));
                                                            }

                                                            throw std::string("invalid argument #1 for function \"tan\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> input{new NativeFunction(0, "input", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                        {                                                             
                                                            std::string str;

                                                            std::getline(std::cin, str);

                                                            sh_ptr<Environment> members(new Environment({}, String->members));

                                                            return sh_ptr<class String>(new (class String)(str, members)); })};

        sh_ptr<NativeFunction> print{new NativeFunction(1, "print", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                        {                                                             
                                                            sh_ptr<Value> data = args.at(0);

                                                            std::cout << data->str() << '\n';

                                                            return sh_ptr<Null>(new Null()); })};

        sh_ptr<NativeFunction> toInt{new NativeFunction(1, "toInt", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                        {                                                             
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Int> intVal = std::dynamic_pointer_cast<Int>(value);

                                                            if (intVal)
                                                            {
                                                                return intVal;
                                                            }

                                                            sh_ptr<Double> doubleVal = std::dynamic_pointer_cast<Double>(value);

                                                            if (doubleVal)
                                                            {
                                                                return sh_ptr<Int>(new Int(doubleVal->value));
                                                            }

                                                            sh_ptr<class String> stringVal = std::dynamic_pointer_cast<class String>(value);

                                                            if (stringVal)
                                                            {
                                                                try 
                                                                {
                                                                    return sh_ptr<Int>(new Int(std::stoi(stringVal->string)));
                                                                } 
                                                                catch(const std::invalid_argument &e) 
                                                                {
                                                                    throw std::string("invalid literal for function \"toInt\": \"" + stringVal->string + "\"");
                                                                    return sh_ptr<Value>();
                                                                }
                                                            }

                                                            throw std::string("invalid argument #1 for function \"toInt\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> toDouble{new NativeFunction(1, "toDouble", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           {                                                             
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<Int> intVal = std::dynamic_pointer_cast<Int>(value);

                                                            if (intVal)
                                                            {
                                                                return sh_ptr<Double>(new Double(intVal->value));;
                                                            }

                                                            sh_ptr<Double> doubleVal = std::dynamic_pointer_cast<Double>(value);

                                                            if (doubleVal)
                                                            {
                                                                return doubleVal;
                                                            }

                                                            sh_ptr<class String> stringVal = std::dynamic_pointer_cast<class String>(value);

                                                            if (stringVal)
                                                            {
                                                                try 
                                                                {
                                                                    return sh_ptr<Int>(new Int(std::stod(stringVal->string)));
                                                                }
                                                                catch(const std::invalid_argument &e) 
                                                                {
                                                                    throw std::string("invalid literal for function \"toDouble\": \"" + stringVal->string + "\"");
                                                                    return sh_ptr<Value>();
                                                                }
                                                            }

                                                            throw std::string("invalid argument #1 for function \"toDouble\"");
                                                            return sh_ptr<Value>(); })};

        sh_ptr<NativeFunction> toStr{new NativeFunction(1, "toStr", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           {                                                             
                                                            sh_ptr<Value> value = args.at(0);

                                                            sh_ptr<class String> stringVal = std::dynamic_pointer_cast<class String>(value);

                                                            if (stringVal)
                                                            {
                                                                return stringVal;
                                                            }                                                        

                                                            sh_ptr<Environment> members(new Environment({}, String->members));

                                                            return sh_ptr<class String>(new (class String)(value->str(), members)); })};

        sh_ptr<NativeFunction> isInt{new NativeFunction(1, "isInt", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           {                                                             
                                                            sh_ptr<Value> value = args.at(0);
                                                            
                                                            return sh_ptr<Boolean>(new Boolean((bool)std::dynamic_pointer_cast<Int>(value))); })};

        sh_ptr<NativeFunction> isDouble{new NativeFunction(1, "isDouble", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           {                                                             
                                                            sh_ptr<Value> value = args.at(0);
                                                            
                                                            return sh_ptr<Boolean>(new Boolean((bool)std::dynamic_pointer_cast<Double>(value))); })};

        sh_ptr<NativeFunction> isObject{new NativeFunction(1, "isObject", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           {                                                             
                                                            sh_ptr<Value> value = args.at(0);
                                                            
                                                            return sh_ptr<Boolean>(new Boolean((bool)std::dynamic_pointer_cast<class Object>(value))); })};

        sh_ptr<NativeFunction> isFunction{new NativeFunction(1, "isFunction", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           {                                                             
                                                            sh_ptr<Value> value = args.at(0);
                                                            
                                                            return sh_ptr<Boolean>(new Boolean((bool)std::dynamic_pointer_cast<class Function>(value))); })};

        sh_ptr<NativeFunction> isStr{new NativeFunction(1, "isStr", sh_ptr<Environment>(new Environment({}, Function->members)), [](std::vector<sh_ptr<Value>> args) -> sh_ptr<Value>
                                                           {                                                             
                                                            sh_ptr<Value> value = args.at(0);
                                                            
                                                            return sh_ptr<Boolean>(new Boolean((bool)std::dynamic_pointer_cast<class String>(value))); })};
    };
}

#endif