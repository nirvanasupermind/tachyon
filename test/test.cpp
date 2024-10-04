
#include <iostream>
#include <string>
#include <map>

double to_tachyon_object(std::map<std::string, double> map) {
    std::map<std::string, double>* ptr = &map;
    double result;
    std::memcpy(&result, &ptr, sizeof(ptr));
    return result;
}

std::map<std::string, double> from_tachyon_object(double obj) {
    std::map<std::string, double>* ptr;
    std::memcpy(&ptr, &obj, sizeof(ptr));
    std::cout << "new ptr " << ptr << '\n';
    return *ptr;
}

double tachyon_object_get(double obj, const std::string& key) {
    std::map<std::string, double> map = from_tachyon_object(obj);
    if (map.count(key) != 0) {
        return map.at(key);
    }
    else {
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
    std::memcpy(&dbl, &ptr, sizeof(ptr));
    return to_tachyon_object({ {"str",dbl},{"proto",String} });
}

std::string from_tachyon_string(double str) {
    auto whatever = from_tachyon_object(str);
    std::cout << "WE MADE IT!!" << '\n';
    return "HAHA";
    // double packed_addr = from_tachyon_object(str).at("str");
    // std::string* ptr;
    // std::memcpy(&ptr, &packed_addr, sizeof(ptr));
    // return *ptr;
}

double Vector = to_tachyon_object({});

double to_tachyon_vector(std::vector<double> vec) {
    std::vector<double>* ptr = &vec;
    double dbl;
    std::memcpy(&dbl, &ptr, sizeof(ptr));
    return to_tachyon_object({ {"_vec",dbl},{"proto",Vector} });
}

std::vector<double> from_tachyon_vector(double vec) {
    double dbl = from_tachyon_object(vec).at("_vec");
    std::vector<double>* ptr;
    std::memcpy(&ptr, &dbl, sizeof(ptr));
    return *ptr;
}

int main() {
    auto a = from_tachyon_object(to_tachyon_string("Hello World"));
    std::cout << "WE MADE IT" << '\n';
    return 0;
}