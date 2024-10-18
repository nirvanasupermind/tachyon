
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <random>
#include "/usr/local/include/tachyon_stl.h"
    int main(){
tachyon_stl_setup();
uint64_t x = pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({{"prototype",Vector}}), new std::vector<uint64_t>({4261412865ULL,4294967297ULL,4311744513ULL,4328521729ULL,4336910337ULL})));
(*(func_ptr*)(unpack_object(print)->other_data))({(*(func_ptr*)(unpack_object(unpack_object(x)->get("accumulate"))->other_data))({x,4261412865ULL,pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({}), new func_ptr([=] (const std::vector<uint64_t>& _args) -> uint64_t {
uint64_t x= _args.at(0);
uint64_t y= _args.at(1);
{
return pack_number(unpack_number(x)*unpack_number(y));
}
return 1ULL;
})))})});

return 0;
}