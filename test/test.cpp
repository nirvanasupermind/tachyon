#include <functional>
#include <iostream>
#include <string>
#include <variant>
#include <vector>
struct _Nil {};
struct _Func;
struct _make_print_functor {
std::string operator()(double x) const { return std::to_string(x); }
std::string operator()(bool x) const { return x ? "true" : "false"; }
std::string operator()(const std::string& x) const { return x; }
std::string operator()(const _Nil& x) const { return "nil"; }
std::string operator()(const _Func& x) const { return "<function>" ;}
};
using _Val = std::variant<double, bool, std::string, _Nil, _Func>;
struct _Func { std::function<_Val (const std::vector<_Val>&)> func; };
_Val print = _Val(_Func{.func = [](const std::vector<_Val>& params) { 
std::cout << std::visit(_make_print_functor(),params.at(0)) << '\n'; return _Val(_Nil());}});
int main() {_Val fib=_Val(_Func{.func = [&fib](const std::vector<_Val>& params){_Val x=params.at(0);{if (std::get<double>(x)<=1.000000){return _Val(1.000000);}else{return _Val(std::get<double>(std::get<_Func>(fib).func(std::vector<_Val>{_Val(std::get<double>(x)-1.000000)}))+std::get<double>(std::get<_Func>(fib).func(std::vector<_Val>{_Val(std::get<double>(x)-2.000000)})));}}return _Val(_Nil());}});std::get<_Func>(print).func(std::vector<_Val>{std::get<_Func>(fib).func(std::vector<_Val>{_Val(7.000000)})}); return 0; }