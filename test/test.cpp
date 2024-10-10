
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>

class TachyonVal {
public:
    double num;
    TachyonVal() {
        this->num = 0.0;
    }
    TachyonVal(double num) {
        this->num = num;
    }
};

using func_ptr = std::function<std::shared_ptr<TachyonVal>(const std::vector<std::shared_ptr<TachyonVal> >&)>;


class TachyonObject: public TachyonVal {
public:
    std::unordered_map<std::string, std::shared_ptr<TachyonVal> > map;
    std::shared_ptr<TachyonObject> proto;

    TachyonObject(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map) {
        this->map = map;
    }

    TachyonObject(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto) {
        this->map = map;
        this->proto = proto;
    }
};

class TachyonString: public TachyonObject {
public:
    std::string str;

    TachyonString(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto,
        const std::string& str)
        : TachyonObject(map, proto) {
        this->str = str;
    }
};


class TachyonVector: public TachyonObject {
public:
    std::vector<std::shared_ptr<TachyonObject> > vec;

    TachyonVector(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto,
        const std::vector<std::shared_ptr<TachyonObject> >& vec)
        : TachyonObject(map, proto) {
        this->vec = vec;
    }
};

class TachyonFunction: public TachyonObject {
public:
    func_ptr func;

    TachyonFunction(const std::unordered_map<std::string, std::shared_ptr<TachyonVal> >& map, const std::shared_ptr<TachyonObject>& proto,
        const func_ptr& func)
        : TachyonObject(map, proto) {
        this->func = func;
    }
};


std::shared_ptr<TachyonObject> String = std::make_shared<TachyonObject>(TachyonObject({}));
std::shared_ptr<TachyonObject> Vector = std::make_shared<TachyonObject>(TachyonObject({}));
std::shared_ptr<TachyonObject> Function = std::make_shared<TachyonObject>(TachyonObject({}));
std::shared_ptr<TachyonVal> print = std::make_shared<TachyonFunction>(TachyonFunction({}, Function, [] (const std::vector<std::shared_ptr<TachyonVal> > &args) {
    std::cout << args.at(0)->num << '\n';
    return std::make_shared<TachyonVal>(TachyonVal(0.0));
}));

    int main(){
std::static_pointer_cast<TachyonFunction>(print)->func({std::make_shared<TachyonVal>(TachyonVal(5))});

return 0;
}