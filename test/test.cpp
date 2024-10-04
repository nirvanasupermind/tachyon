

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
        if(this->map.count("prototype") == 0) {
            return map.at(key);
        } else {
            return TachyonObject::from_double(this->map.at("prototype"))->get(key);
        }
    }

    void set(const std::string& key, double val) {
        map[key] = val;
    }

    double to_double() const {
        double result;
        std::memcpy(&result, this, sizeof(result));
        return result;
    }

    static std::shared_ptr<TachyonObject> from_double(double d) {
        TachyonObject* result;
        std::memcpy(&result, &d, sizeof(result));
        return std::shared_ptr<TachyonObject>(result);
    }
};

double String = TachyonObject({}).to_double();
double Vector = TachyonObject({}).to_double();
double Function = TachyonObject({}).to_double();
double System = TachyonObject({}).to_double();
void setup() {
TachyonObject::from_double(System)->set("printDouble", (TachyonObject({{"prototype",Function}},[](const std::vector<double>& args) {
    double x = args.at(0);
    std::cout << x << '\n';
}).to_double()));
}

    int main(){
setup();
;

return 0;
}