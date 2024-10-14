
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
uint64_t x = (*(func_ptr*)(unpack_object(unpack_object(Complex)->get("fromComponents"))->other_data))({Complex,2130706433ULL,2130706433ULL});
(*(func_ptr*)(unpack_object(print)->other_data))({pack_number(unpack_number((*(func_ptr*)(unpack_object(unpack_object(x)->get("arg"))->other_data))({x}))/unpack_number(unpack_object(Math)->get("PI")))});

return 0;
}