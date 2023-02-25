#ifndef ERIS_H
#define ERIS_H

#include <cassert>
#include <cmath>
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <random>
#include <chrono>
#include <ctime>

// A copy of this file will be included in every transpiled programs

class ErisObject;
class ErisFunc;

// Tagged union
class ErisVal {
public:
    enum {
        NIL,
        NUM,
        BOOL,
        CHAR,
        OBJECT
    } tag;

    union {
        double n;
        bool b;
        char c;
        ErisObject* o;
    };

    ErisVal() = default;

    static ErisVal make_nil();
    static ErisVal make_num(double n);
    static ErisVal make_bool(bool b);
    static ErisVal make_char(char c);
    static ErisVal make_string(const std::string& s);
    static ErisVal make_object(const std::map<std::string, ErisVal>& map);
    static ErisVal make_func(const std::function<ErisVal(std::vector<ErisVal>)>& f);
    ErisVal operator+() const;
    ErisVal operator-() const;
    ErisVal operator+(const ErisVal& other) const;
    ErisVal operator-(const ErisVal& other) const;
    ErisVal operator*(const ErisVal& other) const;
    ErisVal operator/(const ErisVal& other) const;
    ErisVal operator%(const ErisVal& other) const;
    ErisVal operator<<(const ErisVal& other) const;
    ErisVal operator>>(const ErisVal& other) const;
    ErisVal operator&(const ErisVal& other) const;
    ErisVal operator|(const ErisVal& other) const;
    ErisVal operator^(const ErisVal& other) const;
    ErisVal operator&&(const ErisVal& other) const;
    ErisVal operator||(const ErisVal& other) const;
    ErisVal operator<(const ErisVal& other) const;
    ErisVal operator<=(const ErisVal& other) const;
    ErisVal operator>(const ErisVal& other) const;
    ErisVal operator>=(const ErisVal& other) const;
    ErisVal operator==(const ErisVal& other) const;
    ErisVal operator!=(const ErisVal& other) const;
    ErisVal operator()(const std::vector<ErisVal>& args);
    std::string str() const;
};

class ErisObject {
public:
    std::map<std::string, ErisVal> map{};
    ErisObject() = default;
    ErisObject(const std::map<std::string, ErisVal>& map);
    ErisVal get(const std::string& key) const;
    ErisVal set(const std::string& key, const ErisVal& val);
    virtual bool is_str() const;
};

class ErisFunc: public ErisObject {
public:
    std::function<ErisVal(std::vector<ErisVal>)> f;
    ErisFunc(const std::function<ErisVal(std::vector<ErisVal>)>& f);
};

class ErisString: public ErisObject {
public:
    std::string s;
    ErisString(const std::string& s);
    bool is_str() const;
};

ErisVal ErisVal::make_nil() {
    ErisVal result;
    result.tag = NIL;
    return result;
}

ErisVal ErisVal::make_num(double n) {
    ErisVal result;
    result.tag = NUM;
    result.n = n;
    return result;
}

ErisVal ErisVal::make_bool(bool b) {
    ErisVal result;
    result.tag = BOOL;
    result.b = b;
    return result;
}


ErisVal ErisVal::make_char(char c) {
    ErisVal result;
    result.tag = CHAR;
    result.c = c;
    return result;
}

ErisVal ErisVal::make_object(const std::map<std::string, ErisVal>& map) {
    ErisVal result;
    result.tag = ErisVal::OBJECT;
    result.o = new ErisObject(map);
    return result;
}

ErisVal ErisVal::make_func(const std::function<ErisVal(std::vector<ErisVal>)>& f) {
    ErisVal result;
    result.tag = ErisVal::OBJECT;
    result.o = new ErisFunc(f);
    return result;
}

ErisVal ErisVal::make_string(const std::string& s) {
    ErisVal result;
    result.tag = OBJECT;
    result.o = new ErisString(s);
    return result;
}

ErisVal ErisVal::operator+() const {
    assert(tag == NUM);
    return ErisVal::make_num(+n);
}

ErisVal ErisVal::operator-() const {
    assert(tag == NUM);
    return ErisVal::make_num(-n);
}

ErisVal ErisVal::operator+(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num(n + other.n);
}

ErisVal ErisVal::operator-(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num(n - other.n);
}

ErisVal ErisVal::operator*(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num(n * other.n);
}

ErisVal ErisVal::operator/(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num(n / other.n);
}

ErisVal ErisVal::operator%(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num(std::fmod(n, other.n));
}

ErisVal ErisVal::operator<<(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num((int)n << (int)other.n);
}

ErisVal ErisVal::operator>>(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num((int)n >> (int)other.n);
}

ErisVal ErisVal::operator&(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num((int)n & (int)other.n);
}

ErisVal ErisVal::operator|(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num((int)n | (int)other.n);
}

ErisVal ErisVal::operator^(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_num((int)n ^ (int)other.n);
}

ErisVal ErisVal::operator&&(const ErisVal& other) const {
    assert(tag == BOOL && other.tag == BOOL);
    return ErisVal::make_bool(n && other.n);
}

ErisVal ErisVal::operator||(const ErisVal& other) const {
    assert(tag == BOOL && other.tag == BOOL);
    return ErisVal::make_bool(n || other.n);
}

ErisVal ErisVal::operator<(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_bool(n < other.n);
}

ErisVal ErisVal::operator<=(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_bool(n <= other.n);
}

ErisVal ErisVal::operator>(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_bool(n > other.n);
}

ErisVal ErisVal::operator>=(const ErisVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return ErisVal::make_bool(n >= other.n);
}

ErisVal ErisVal::operator==(const ErisVal& other) const {
    if (tag == NIL) {
        return ErisVal::make_bool(other.tag == NIL);
    }
    else if (tag == NUM) {
        return ErisVal::make_bool(other.tag == NUM && n == other.n);
    }
    else if (tag == BOOL) {
        return ErisVal::make_bool(other.tag == BOOL && b == other.b);
    } else if(tag == OBJECT) {
        return ErisVal::make_bool(other.tag == OBJECT && o == other.o);
    }
}

ErisVal ErisVal::operator!=(const ErisVal& other) const {
    return ErisVal::make_bool(!(operator==(other)).b);
}

ErisVal ErisVal::operator()(const std::vector<ErisVal>& args) {
    assert(tag == OBJECT);
    return static_cast<ErisFunc*>(o)->f(args);
}

std::string ErisVal::str() const {
    if (tag == NIL) {
        return "nil";
    }
    else if (tag == NUM) {
        std::ostringstream oss;
        oss << n;
        return oss.str();
    }
    else if (tag == BOOL) {
        return b ? "true" : "false";
    }
    else if (tag == CHAR) {
        return std::string(1, c);
    }
    else if (tag == OBJECT) {
        if(o->is_str()) {
            return static_cast<ErisString*>(o)->s;
        }
        std::ostringstream oss;
        oss << o;
        return oss.str();
    }
}

ErisObject::ErisObject(const std::map<std::string, ErisVal>& map)
    : map(map) {
}

ErisVal ErisObject::get(const std::string& key) const {
    if (!(map.count(key)) && map.count("proto")) {
        return map.at("proto").o->get(key);
    }
    else {
        return map.at(key);
    }
}

ErisVal ErisObject::set(const std::string& key, const ErisVal& val) {
    map[key] = val;
    return val;
}

bool ErisObject::is_str() const {
    return false;
}

ErisFunc::ErisFunc(const std::function<ErisVal(std::vector<ErisVal>)>& f)
    : f(f) {

}

ErisString::ErisString(const std::string& s)
    : s(s) {

}

bool ErisString::is_str() const {
    return true;
}

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

ErisVal System = ErisVal::make_object({
    {"print", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        std::cout << args.at(1).str() << '\n';
        return ErisVal::make_nil();
    })},
    {"exit", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        std::exit(0);
        return ErisVal::make_nil();
    })},
    {"time", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        return ErisVal::make_num(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    })}
    });

ErisVal Math = ErisVal::make_object({
    {"PI", ErisVal::make_num(3.14159265358979323846)},
    {"E", ErisVal::make_num(2.7182818284590452354)},
    {"sin", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::sin(args.at(1).n));
    })},
    {"cos", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::cos(args.at(1).n));
    })},
    {"tan", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::tan(args.at(1).n));
    })},
    {"asin", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::asin(args.at(1).n));
    })},
    {"acos", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::acos(args.at(1).n));
    })},
    {"atan", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::atan(args.at(1).n));
    })},
    {"atan2", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        assert(args.at(2).tag == ErisVal::NUM);
        return ErisVal::make_num(std::atan2(args.at(1).n, args.at(2).n));
    })},
    {"exp", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::exp(args.at(1).n));
    })},
    {"log", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::log(args.at(1).n));
    })},
    {"sqrt", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::sqrt(args.at(1).n));
    })},
    {"pow", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        assert(args.at(2).tag == ErisVal::NUM);
        return ErisVal::make_num(std::pow(args.at(1).n, args.at(2).n));
    })},
    {"ceil", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::ceil(args.at(1).n));
    })},
    {"floor", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::floor(args.at(1).n));
    })},
    {"round", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(1).tag == ErisVal::NUM);
        return ErisVal::make_num(std::round(args.at(1).n));
    })},
    {"rand", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        return ErisVal::make_num(dist(mt));
    })}
    });

#endif // ERIS_H