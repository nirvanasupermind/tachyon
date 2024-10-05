
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
class TachyonObject {
public:
    std::map<std::string, double> map;
    std::string str;
    std::vector<double> vec;
    std::function<double(const std::vector<double>&)> func;
    TachyonObject(const std::map<std::string, double>& map) {
        this->map = map;
    }
    TachyonObject(const std::map<std::string, double>& map, const std::string& str) {
        this->map = map;
        this->str = str;
    }
    
    TachyonObject(const std::map<std::string, double>& map, const std::vector<double>& vec) {
        this->map = map;
        this->vec = vec;
    }
    
    TachyonObject(const std::map<std::string, double>& map, const std::function<double(const std::vector<double>&)>& func) {
        this->map = map;
        this->func = func;
    }

    double get(const std::string& key) {
    if(map.count(key) == 0) {
    return TachyonObject::from_double(map.at("proto"))->get(key);
    } else {
    return map.at(key);
    }
    }

    void set(const std::string& key, double val) {
        map[key] = val;
    }

    double to_double() {
        TachyonObject* temp = this;
        double result;
        std::memcpy(&result, &temp, sizeof(result));
        return result;
    }

    static std::shared_ptr<TachyonObject> from_double(double d) {
        TachyonObject* result;
        std::memcpy(&result, &d, sizeof(result));
        return std::shared_ptr<TachyonObject>(result);
    }

};


double make_tachyon_object(const std::map<std::string, double>& map) {
    return std::make_shared<TachyonObject>(TachyonObject(map))->to_double();
}

double tachyon_object_get(double obj, const std::string& key){
    return TachyonObject::from_double(obj)->get(key);
}

void tachyon_object_set(double obj, const std::string& key, double val){
    TachyonObject::from_double(obj)->set(key, val);
}

double Function = make_tachyon_object({});

double make_tachyon_function(const std::function<double(const std::vector<double>&)>& func) {
    return std::make_shared<TachyonObject>(TachyonObject({{"prototype", Function}}, func))->to_double();
}

double String = make_tachyon_object({
    {"toString", make_tachyon_function([](const std::vector<double>& args) {
        return args.at(0);
    })}
});

double Vector = make_tachyon_object({});

double make_tachyon_string(const std::string& str) {
    return std::make_shared<TachyonObject>(TachyonObject({{"prototype", String}}, str))->to_double();
}

double make_tachyon_vector(const std::vector<double>& vec) {
    return std::make_shared<TachyonObject>(TachyonObject({{"prototype", Vector}}, vec))->to_double();
}

double System = make_tachyon_object({});
    int main(){
setup();
make_tachyon_string("Hello world");

return 0;
}