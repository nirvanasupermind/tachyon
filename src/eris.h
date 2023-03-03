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
#include <thread>
#include <algorithm>
#include <fstream>

// A copy of this file will be included in every transpiled programs

class ErisObject;

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
    static ErisVal make_object(const std::map<std::string, ErisVal>& map);
    static ErisVal make_str(const std::string& s);
    static ErisVal make_vec(const std::vector<ErisVal>& v);
    static ErisVal make_func(const std::function<ErisVal(std::vector<ErisVal>)>& f);
    static ErisVal make_thread(std::thread* t);
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

extern ErisVal String;
extern ErisVal Vec;
extern ErisVal Func;
extern ErisVal Thread;

class ErisObject {
public:
    std::map<std::string, ErisVal> map{};
    ErisObject() = default;
    ErisObject(const std::map<std::string, ErisVal>& map);
    ErisVal get(const std::string& key) const;
    ErisVal set(const std::string& key, const ErisVal& val);
};


class ErisString: public ErisObject {
public:
    std::string s;
    ErisString(const std::string& s);
};

class ErisVec: public ErisObject {
public:
    std::vector<ErisVal> v;
    ErisVec(const std::vector<ErisVal>& v);
};

class ErisFunc: public ErisObject {
public:
    std::function<ErisVal(std::vector<ErisVal>)> f;
    ErisFunc(const std::function<ErisVal(std::vector<ErisVal>)>& f);
};

class ErisThread: public ErisObject {
public:
    std::thread* t;
    ErisThread(std::thread* t);
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

ErisVal ErisVal::make_str(const std::string& s) {
    ErisVal result;
    result.tag = OBJECT;
    result.o = new ErisString(s);
    return result;
}

ErisVal ErisVal::make_thread(std::thread* t) {
    ErisVal result;
    result.tag = OBJECT;
    result.o = new ErisThread(t);
    return result;
}

ErisVal ErisVal::make_vec(const std::vector<ErisVal>& v) {
    ErisVal result;
    result.tag = ErisVal::OBJECT;
    result.o = new ErisVec(v);
    return result;
}

ErisVal ErisVal::make_func(const std::function<ErisVal(std::vector<ErisVal>)>& f) {
    ErisVal result;
    result.tag = ErisVal::OBJECT;
    result.o = new ErisFunc(f);
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
    }
    else if (tag == OBJECT) {
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
        if ((o->map.at("proto") == String).b) {
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

ErisString::ErisString(const std::string& s)
    : s(s) {
    set("proto", String);
}

ErisVec::ErisVec(const std::vector<ErisVal>& v)
    : v(v) {
    set("proto", Vec);
}

ErisFunc::ErisFunc(const std::function<ErisVal(std::vector<ErisVal>)>& f)
    : f(f) {
    set("proto", Func);
}

ErisThread::ErisThread(std::thread* t)
    : t(t) {
    set("proto", Thread);
}

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

ErisVal System = ErisVal::make_object({
    {"print", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        std::cout << args.at(1).str() << '\n';
        return ErisVal::make_nil();
    })},
    {"input", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        std::string input;
        std::cin >> input;
        return ErisVal::make_str(input);
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
        assert(args.at(1).tag == ErisVal::NUM && args.at(2).tag == ErisVal::NUM);
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
        assert(args.at(1).tag == ErisVal::NUM && args.at(2).tag == ErisVal::NUM);
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

ErisVal String = ErisVal::make_object({
    {"length", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        return ErisVal::make_num(str.length());
    })},
    {"at", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT && args.at(1).tag == ErisVal::NUM);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        return ErisVal::make_char(str.at(args.at(1).n));
    })},
    {"first", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        return ErisVal::make_char(str.front());
    })},
    {"last", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        return ErisVal::make_char(str.back());
    })},
    {"find", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT && args.at(1).tag == ErisVal::OBJECT);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        std::string str2 = static_cast<ErisString*>(args.at(1).o)->s;
        return ErisVal::make_num(str.find(str2));
    })},
    {"contains", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT && args.at(1).tag == ErisVal::OBJECT);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        std::string str2 = static_cast<ErisString*>(args.at(1).o)->s;
        return ErisVal::make_bool(str.find(str2) != std::string::npos);
    })},
    {"substr", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT && args.at(1).tag == ErisVal::NUM && args.at(2).tag == ErisVal::NUM);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        return ErisVal::make_str(str.substr(args.at(1).n, args.at(2).n));
    })},
    {"concat", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT && args.at(1).tag == ErisVal::OBJECT);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        std::string str2 = static_cast<ErisString*>(args.at(1).o)->s;
        return ErisVal::make_str(str + str2);
    })},
    {"split", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        std::string str = static_cast<ErisString*>(args.at(0).o)->s;
        std::string str2 = static_cast<ErisString*>(args.at(1).o)->s;
        std::string str3 = str;
        std::vector<ErisVal> list;
        std::size_t pos = 0;
        std::string token;
        while ((pos = str3.find(str2)) != std::string::npos) {
            token = str3.substr(0, pos);
            list.push_back(ErisVal::make_str(token));
            str3.erase(0, pos + str2.length());
        }
        list.push_back(ErisVal::make_str(str3));
        return ErisVal::make_vec(list);
    })},
    {"from", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        return ErisVal::make_str(args.at(1).str());
    })}
    });

ErisVal Vec = ErisVal::make_object({
    {"length", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        std::vector<ErisVal> vec = static_cast<ErisVec*>(args.at(0).o)->v;
        return ErisVal::make_num(vec.size());
    })},
    {"at", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        assert(args.at(1).tag == ErisVal::NUM);
        std::vector<ErisVal> vec = static_cast<ErisVec*>(args.at(0).o)->v;
        return vec.at(args.at(1).n);
    })},
    {"first", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        std::vector<ErisVal> vec = static_cast<ErisVec*>(args.at(0).o)->v;
        return vec.front();
    })},
    {"last", ErisVal::make_func([](const std::vector<ErisVal>& args) {
        assert(args.at(0).tag == ErisVal::OBJECT);
        std::vector<ErisVal> vec = static_cast<ErisVec*>(args.at(0).o)->v;
        return vec.back();
    })},
    {"push", ErisVal::make_func([](const std::vector<ErisVal>& args) {
    assert(args.at(0).tag == ErisVal::OBJECT);
    std::vector<ErisVal> vec = static_cast<ErisVec*>(args.at(0).o)->v;
    vec.push_back(args.at(1));
    return ErisVal::make_nil();
    })},
    {"pop", ErisVal::make_func([](const std::vector<ErisVal>& args) {
    assert(args.at(0).tag == ErisVal::OBJECT);
    std::vector<ErisVal> vec = static_cast<ErisVec*>(args.at(0).o)->v;
    vec.pop_back();
    return ErisVal::make_nil();
    })},
    {"subvec", ErisVal::make_func([](const std::vector<ErisVal>& args) {
    assert(args.at(0).tag == ErisVal::OBJECT && args.at(1).tag == ErisVal::NUM && args.at(2).tag == ErisVal::NUM);
    std::vector<ErisVal> vec = static_cast<ErisVec*>(args.at(0).o)->v;
    return ErisVal::make_vec({vec.begin() + args.at(1).n, vec.begin() + args.at(1).n + args.at(2).n});
    })}
    });

ErisVal Func = ErisVal::make_object({});

ErisVal Thread = ErisVal::make_object({
    {"create", ErisVal::make_func([](const std::vector<ErisVal>& args) {
    assert(args.at(1).tag == ErisVal::OBJECT);

    return ErisVal::make_thread(new std::thread([args] () {
        return static_cast<ErisFunc*>(args.at(1).o)->f({});
    }));
    })},
    {"join", ErisVal::make_func([](const std::vector<ErisVal>& args) {
    assert(args.at(0).tag == ErisVal::OBJECT);
    std::thread* thr = static_cast<ErisThread*>(args.at(0).o)->t;
    thr->join();
    return ErisVal::make_nil();
    })}
    });

ErisVal FileSystem = ErisVal::make_object({
    {"read", ErisVal::make_func([](const std::vector<ErisVal>& args) {
    assert(args.at(1).tag == ErisVal::OBJECT);
    std::string path = static_cast<ErisString*>(args.at(1).o)->s;
    std::ifstream in_file;
    in_file.open(path);
    std::stringstream strStream;
    strStream << in_file.rdbuf();
    std::string text = strStream.str();
    return ErisVal::make_str(text);
    })},
    {"write", ErisVal::make_func([](const std::vector<ErisVal>& args) {
    assert(args.at(1).tag == ErisVal::OBJECT && args.at(2).tag == ErisVal::OBJECT);
    std::string path = static_cast<ErisString*>(args.at(1).o)->s;
    std::string str = static_cast<ErisString*>(args.at(2).o)->s;
    std::ofstream out_file;
    out_file.open(path);
    out_file << str;
    return ErisVal::make_nil();
    })}
    });

#endif // ERIS_H