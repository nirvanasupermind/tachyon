
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
uint64_t i = 1ULL;
i = pack_number(unpack_number(i) + 1.0);
(*(func_ptr*)(unpack_object(print)->other_data))({i});

return 0;
}