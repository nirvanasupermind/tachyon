#ifndef TACHYON_H
#define TACHYON_H

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
#include <cstdint>

// A copy of this file will be included in every transpiled programs

class TachyonObject;

// Tagged union
class TachyonVal {
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
        TachyonObject* o;
    };

    TachyonVal() = default;

    static TachyonVal make_nil();
    static TachyonVal make_num(double n);
    static TachyonVal make_bool(bool b);
    static TachyonVal make_char(char c);
    static TachyonVal make_object(const std::map<std::string, TachyonVal>& map);
    static TachyonVal make_str(const std::string& s);
    static TachyonVal make_vec(const std::vector<TachyonVal>& v);
    static TachyonVal make_func(const std::function<TachyonVal(std::vector<TachyonVal>)>& f);
    static TachyonVal make_thread(std::thread* t);
    TachyonVal operator+() const;
    TachyonVal operator-() const;
    TachyonVal operator+(const TachyonVal& other) const;
    TachyonVal operator-(const TachyonVal& other) const;
    TachyonVal operator*(const TachyonVal& other) const;
    TachyonVal operator/(const TachyonVal& other) const;
    TachyonVal operator%(const TachyonVal& other) const;
    TachyonVal operator<<(const TachyonVal& other) const;
    TachyonVal operator>>(const TachyonVal& other) const;
    TachyonVal operator&(const TachyonVal& other) const;
    TachyonVal operator|(const TachyonVal& other) const;
    TachyonVal operator^(const TachyonVal& other) const;
    TachyonVal operator&&(const TachyonVal& other) const;
    TachyonVal operator||(const TachyonVal& other) const;
    TachyonVal operator<(const TachyonVal& other) const;
    TachyonVal operator<=(const TachyonVal& other) const;
    TachyonVal operator>(const TachyonVal& other) const;
    TachyonVal operator>=(const TachyonVal& other) const;
    TachyonVal operator==(const TachyonVal& other) const;
    TachyonVal operator!=(const TachyonVal& other) const;
    TachyonVal operator()(const std::vector<TachyonVal>& args);
    std::string str() const;
};

extern TachyonVal String;
extern TachyonVal Vec;
extern TachyonVal Func;
extern TachyonVal Thread;

class TachyonObject {
public:
    std::map<std::string, TachyonVal> map{};
    TachyonObject() = default;
    TachyonObject(const std::map<std::string, TachyonVal>& map);
    TachyonVal get(const std::string& key) const;
    TachyonVal set(const std::string& key, const TachyonVal& val);
};


class TachyonString: public TachyonObject {
public:
    std::string s;
    TachyonString(const std::string& s);
};

class TachyonVec: public TachyonObject {
public:
    std::vector<TachyonVal> v;
    TachyonVec(const std::vector<TachyonVal>& v);
};

class TachyonFunc: public TachyonObject {
public:
    std::function<TachyonVal(std::vector<TachyonVal>)> f;
    TachyonFunc(const std::function<TachyonVal(std::vector<TachyonVal>)>& f);
};

class TachyonThread: public TachyonObject {
public:
    std::thread* t;
    TachyonThread(std::thread* t);
};

TachyonVal TachyonVal::make_nil() {
    TachyonVal result;
    result.tag = NIL;
    return result;
}

TachyonVal TachyonVal::make_num(double n) {
    TachyonVal result;
    result.tag = NUM;
    result.n = n;
    return result;
}

TachyonVal TachyonVal::make_bool(bool b) {
    TachyonVal result;
    result.tag = BOOL;
    result.b = b;
    return result;
}

TachyonVal TachyonVal::make_char(char c) {
    TachyonVal result;
    result.tag = CHAR;
    result.c = c;
    return result;
}

TachyonVal TachyonVal::make_object(const std::map<std::string, TachyonVal>& map) {
    TachyonVal result;
    result.tag = TachyonVal::OBJECT;
    result.o = new TachyonObject(map);
    return result;
}

TachyonVal TachyonVal::make_str(const std::string& s) {
    TachyonVal result;
    result.tag = OBJECT;
    result.o = new TachyonString(s);
    return result;
}

TachyonVal TachyonVal::make_thread(std::thread* t) {
    TachyonVal result;
    result.tag = OBJECT;
    result.o = new TachyonThread(t);
    return result;
}

TachyonVal TachyonVal::make_vec(const std::vector<TachyonVal>& v) {
    TachyonVal result;
    result.tag = TachyonVal::OBJECT;
    result.o = new TachyonVec(v);
    return result;
}

TachyonVal TachyonVal::make_func(const std::function<TachyonVal(std::vector<TachyonVal>)>& f) {
    TachyonVal result;
    result.tag = TachyonVal::OBJECT;
    result.o = new TachyonFunc(f);
    return result;
}

TachyonVal TachyonVal::operator+() const {
    assert(tag == NUM);
    return TachyonVal::make_num(+n);
}

TachyonVal TachyonVal::operator-() const {
    assert(tag == NUM);
    return TachyonVal::make_num(-n);
}

TachyonVal TachyonVal::operator+(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num(n + other.n);
}

TachyonVal TachyonVal::operator-(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num(n - other.n);
}

TachyonVal TachyonVal::operator*(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num(n * other.n);
}

TachyonVal TachyonVal::operator/(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num(n / other.n);
}

TachyonVal TachyonVal::operator%(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num(std::fmod(n, other.n));
}

TachyonVal TachyonVal::operator<<(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num((int64_t)n << (int64_t)other.n);
}

TachyonVal TachyonVal::operator>>(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num((int64_t)n >> (int64_t)other.n);
}

TachyonVal TachyonVal::operator&(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num((int64_t)n && (int64_t)other.n);
}

TachyonVal TachyonVal::operator|(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num((int64_t)n | (int64_t)other.n);
}

TachyonVal TachyonVal::operator^(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_num((int64_t)n ^ (int64_t)other.n);
}

TachyonVal TachyonVal::operator&&(const TachyonVal& other) const {
    assert(tag == BOOL && other.tag == BOOL);
    return TachyonVal::make_bool(n && other.n);
}

TachyonVal TachyonVal::operator||(const TachyonVal& other) const {
    assert(tag == BOOL && other.tag == BOOL);
    return TachyonVal::make_bool(n || other.n);
}

TachyonVal TachyonVal::operator<(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_bool(n < other.n);
}

TachyonVal TachyonVal::operator<=(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_bool(n <= other.n);
}

TachyonVal TachyonVal::operator>(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_bool(n > other.n);
}

TachyonVal TachyonVal::operator>=(const TachyonVal& other) const {
    assert(tag == NUM && other.tag == NUM);
    return TachyonVal::make_bool(n >= other.n);
}

TachyonVal TachyonVal::operator==(const TachyonVal& other) const {
    if (tag == NIL) {
        return TachyonVal::make_bool(other.tag == NIL);
    }
    else if (tag == NUM) {
        return TachyonVal::make_bool(other.tag == NUM && n == other.n);
    }
    else if (tag == BOOL) {
        return TachyonVal::make_bool(other.tag == BOOL && b == other.b);
    }
    else if (tag == OBJECT) {
        return TachyonVal::make_bool(other.tag == OBJECT && o == other.o);
    }
    return TachyonVal::make_nil();
}

TachyonVal TachyonVal::operator!=(const TachyonVal& other) const {
    return TachyonVal::make_bool(!(operator==(other)).b);
}

TachyonVal TachyonVal::operator()(const std::vector<TachyonVal>& args) {
    assert(tag == OBJECT);
    return static_cast<TachyonFunc*>(o)->f(args);
}

std::string TachyonVal::str() const {
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
            return static_cast<TachyonString*>(o)->s;
        }
        std::ostringstream oss;
        oss << o;
        return oss.str();
    }
    return "";
}

TachyonObject::TachyonObject(const std::map<std::string, TachyonVal>& map)
    : map(map) {
}

TachyonVal TachyonObject::get(const std::string& key) const {
    if (!(map.count(key)) && map.count("proto")) {
        return map.at("proto").o->get(key);
    }
    else {
        return map.at(key);
    }
}

TachyonVal TachyonObject::set(const std::string& key, const TachyonVal& val) {
    map[key] = val;
    return val;
}

TachyonString::TachyonString(const std::string& s)
    : s(s) {
    set("proto", String);
}

TachyonVec::TachyonVec(const std::vector<TachyonVal>& v)
    : v(v) {
    set("proto", Vec);
}

TachyonFunc::TachyonFunc(const std::function<TachyonVal(std::vector<TachyonVal>)>& f)
    : f(f) {
    set("proto", Func);
}

TachyonThread::TachyonThread(std::thread* t)
    : t(t) {
    set("proto", Thread);
}

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, 1.0);

TachyonVal System = TachyonVal::make_object({
    {"print", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        std::cout << args.at(1).str() << '\n';
        return TachyonVal::make_nil();
    })},
    {"input", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        std::string input;
        std::cin >> input;
        return TachyonVal::make_str(input);
    })},
    {"exit", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        std::exit(0);
        return TachyonVal::make_nil();
    })},
    {"time", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        return TachyonVal::make_num(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    })},
    {"assert", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::BOOL && args.at(1).b);
        return TachyonVal::make_num(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    })},
    });

TachyonVal Math = TachyonVal::make_object({
    {"PI", TachyonVal::make_num(3.14159265358979323846)},
    {"E", TachyonVal::make_num(2.7182818284590452354)},
    {"sin", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::sin(args.at(1).n));
    })},
    {"cos", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::cos(args.at(1).n));
    })},
    {"tan", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::tan(args.at(1).n));
    })},
    {"asin", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::asin(args.at(1).n));
    })},
    {"acos", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::acos(args.at(1).n));
    })},
    {"atan", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::atan(args.at(1).n));
    })},
    {"atan2", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM && args.at(2).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::atan2(args.at(1).n, args.at(2).n));
    })},
    {"exp", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::exp(args.at(1).n));
    })},
    {"log", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::log(args.at(1).n));
    })},
    {"sqrt", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::sqrt(args.at(1).n));
    })},
    {"pow", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM && args.at(2).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::pow(args.at(1).n, args.at(2).n));
    })},
    {"ceil", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::ceil(args.at(1).n));
    })},
    {"floor", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::floor(args.at(1).n));
    })},
    {"round", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(1).tag == TachyonVal::NUM);
        return TachyonVal::make_num(std::round(args.at(1).n));
    })},
    {"rand", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        return TachyonVal::make_num(dist(mt));
    })}
    });

TachyonVal String = TachyonVal::make_object({
    {"length", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        return TachyonVal::make_num(str.length());
    })},
    {"at", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT && args.at(1).tag == TachyonVal::NUM);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        return TachyonVal::make_char(str.at(args.at(1).n));
    })},
    {"first", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        return TachyonVal::make_char(str.front());
    })},
    {"last", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        return TachyonVal::make_char(str.back());
    })},
    {"find", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT && args.at(1).tag == TachyonVal::OBJECT);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        std::string str2 = static_cast<TachyonString*>(args.at(1).o)->s;
        return TachyonVal::make_num(str.find(str2));
    })},
    {"contains", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT && args.at(1).tag == TachyonVal::OBJECT);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        std::string str2 = static_cast<TachyonString*>(args.at(1).o)->s;
        return TachyonVal::make_bool(str.find(str2) != std::string::npos);
    })},
    {"substr", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT && args.at(1).tag == TachyonVal::NUM && args.at(2).tag == TachyonVal::NUM);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        return TachyonVal::make_str(str.substr(args.at(1).n, args.at(2).n));
    })},
    {"concat", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT && args.at(1).tag == TachyonVal::OBJECT);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        std::string str2 = static_cast<TachyonString*>(args.at(1).o)->s;
        return TachyonVal::make_str(str + str2);
    })},
    {"split", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        std::string str = static_cast<TachyonString*>(args.at(0).o)->s;
        std::string str2 = static_cast<TachyonString*>(args.at(1).o)->s;
        std::string str3 = str;
        std::vector<TachyonVal> list;
        std::size_t pos = 0;
        std::string token;
        while ((pos = str3.find(str2)) != std::string::npos) {
            token = str3.substr(0, pos);
            list.push_back(TachyonVal::make_str(token));
            str3.erase(0, pos + str2.length());
        }
        list.push_back(TachyonVal::make_str(str3));
        return TachyonVal::make_vec(list);
    })},
    {"from", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        return TachyonVal::make_str(args.at(1).str());
    })}
    });

TachyonVal Vec = TachyonVal::make_object({
    {"length", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        std::vector<TachyonVal> vec = static_cast<TachyonVec*>(args.at(0).o)->v;
        return TachyonVal::make_num(vec.size());
    })},
    {"at", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        assert(args.at(1).tag == TachyonVal::NUM);
        std::vector<TachyonVal> vec = static_cast<TachyonVec*>(args.at(0).o)->v;
        return vec.at(args.at(1).n);
    })},
    {"first", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        std::vector<TachyonVal> vec = static_cast<TachyonVec*>(args.at(0).o)->v;
        return vec.front();
    })},
    {"last", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
        assert(args.at(0).tag == TachyonVal::OBJECT);
        std::vector<TachyonVal> vec = static_cast<TachyonVec*>(args.at(0).o)->v;
        return vec.back();
    })},
    {"push", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    assert(args.at(0).tag == TachyonVal::OBJECT);
    std::vector<TachyonVal> vec = static_cast<TachyonVec*>(args.at(0).o)->v;
    vec.push_back(args.at(1));
    return TachyonVal::make_nil();
    })},
    {"pop", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    assert(args.at(0).tag == TachyonVal::OBJECT);
    std::vector<TachyonVal> vec = static_cast<TachyonVec*>(args.at(0).o)->v;
    vec.pop_back();
    return TachyonVal::make_nil();
    })},
    {"subvec", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    assert(args.at(0).tag == TachyonVal::OBJECT && args.at(1).tag == TachyonVal::NUM && args.at(2).tag == TachyonVal::NUM);
    std::vector<TachyonVal> vec = static_cast<TachyonVec*>(args.at(0).o)->v;
    return TachyonVal::make_vec({vec.begin() + args.at(1).n, vec.begin() + args.at(1).n + args.at(2).n});
    })}
    });

TachyonVal Func = TachyonVal::make_object({});

TachyonVal Thread = TachyonVal::make_object({
    {"create", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    assert(args.at(1).tag == TachyonVal::OBJECT);

    return TachyonVal::make_thread(new std::thread([args]() {
        return static_cast<TachyonFunc*>(args.at(1).o)->f({});
    }));
    })},
    {"join", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    assert(args.at(0).tag == TachyonVal::OBJECT);
    std::thread* thr = static_cast<TachyonThread*>(args.at(0).o)->t;
    thr->join();
    return TachyonVal::make_nil();
    })}
    });

TachyonVal FileSystem = TachyonVal::make_object({
    {"read", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    assert(args.at(1).tag == TachyonVal::OBJECT);
    std::string path = static_cast<TachyonString*>(args.at(1).o)->s;
    std::ifstream in_file;
    in_file.open(path);
    std::stringstream strStream;
    strStream << in_file.rdbuf();
    std::string text = strStream.str();
    return TachyonVal::make_str(text);
    })},
    {"write", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    assert(args.at(1).tag == TachyonVal::OBJECT && args.at(2).tag == TachyonVal::OBJECT);
    std::string path = static_cast<TachyonString*>(args.at(1).o)->s;
    std::string str = static_cast<TachyonString*>(args.at(2).o)->s;
    std::ofstream out_file;
    out_file.open(path);
    out_file << str;
    return TachyonVal::make_nil();
    })}
    });

TachyonVal Exception = TachyonVal::make_object({
    {"throw", TachyonVal::make_func([](const std::vector<TachyonVal>& args) {
    std::string msg = static_cast<TachyonString*>(args.at(0).o->get("msg").o)->s;
    throw std::runtime_error(msg);
    return TachyonVal::make_nil();
    })}
    });

#endif // TACHYON_H