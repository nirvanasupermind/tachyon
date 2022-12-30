#include <iostream>
#include <string>
#include <variant>
class _Nil {};
using _Val = std::variant<double, bool, std::string, _Nil>;
int main() {for (_Val i=_Val(0.000000);std::get<bool>(_Val(std::get<double>(i)<100.000000));i=_Val(std::get<double>(i)+1.000000)){} return 0; }