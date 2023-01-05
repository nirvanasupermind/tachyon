#include <functional>
#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <cmath>
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
namespace eris{
_Val print = _Val(_Func{.func = [](const std::vector<_Val>& params){
std::cout << std::visit(_make_print_functor(),params.at(0)) << '\n'; return _Val(_Nil());}});
_Val sin = _Val(_Func{.func = [](const std::vector<_Val>& params){
return _Val(std::sin(std::get<double>(params.at(0))));}});
}
int main() {std::get<_Func>(eris::print).func(std::vector<_Val>{std::get<_Func>(eris::sin).func(std::vector<_Val>{_Val(3.140000)})}); return 0; }