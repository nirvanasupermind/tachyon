
#include <iostream>
#include <string>
#include <map>

double to_tachyon_object(std::map<std::string, double> map) {
    std::map<std::string, double>* ptr = &map;
    double result;
    std::memcpy(&ptr, &result, sizeof(ptr));
    return result;
}

std::map<std::string, double> from_tachyon_object(double dbl) {
    std::map<std::string, double>* ptr;
    std::memcpy(&dbl, &ptr, sizeof(ptr));
    return *ptr;
}

double tachyon_object_get(double obj, const std::string& key) {
    std::map<std::string, double> map = from_tachyon_object(obj);
    if(map.count(key) != 0) {
        return map.at(key);
    } else {
        return from_tachyon_object(map.at("proto")).at(key);
    }
}

void tachyon_object_set(double obj, const std::string& key, double val) {
    std::map<std::string, double> map = from_tachyon_object(obj);
    map[key] = val;
}

double String = to_tachyon_object({});

double to_tachyon_string(std::string str) {
    std::string* ptr = &str;
    double dbl;
    std::memcpy(&ptr, &dbl, sizeof(ptr));
    return to_tachyon_object({{"_str",dbl},{"proto",String}});
}

std::string from_tachyon_string(double str) {
    double dbl = from_tachyon_object(str).at("_str");
    std::string* ptr;
    std::memcpy(&dbl, &ptr, sizeof(ptr));
    return *ptr;
}

double Vector = to_tachyon_object({});

double to_tachyon_vector(std::vector<double> vec) {
    std::vector<double>* ptr = &vec;
    double dbl;
    std::memcpy(&ptr, &dbl, sizeof(ptr));
    return to_tachyon_object({{"_vec",dbl},{"proto",Vector}});
}

std::vector<double> from_tachyon_vector(double vec) {
    double dbl = from_tachyon_object(vec).at("_vec");
    std::vector<double>* ptr;
    std::memcpy(&dbl, &ptr, sizeof(ptr));
    return *ptr;
}
    int main(){
std::cout << from_tachyon_string(to_tachyon_string("a")) << '\n';

return 0;
}