#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include "node.h"
#include "transpiler.h"

namespace tachyon {
    const std::string Transpiler::boilerplate = R"VOG0N(
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

)VOG0N";
    Transpiler::Transpiler(const std::string& filename)
        : filename(filename), included_headers({
            "<cassert>",
            "<cmath>",
            "<functional>",
            "<string>",
            "<vector>",
            "<map>",
            "<iostream>",
            "<sstream>",
            "<random>",
            "<chrono>",
            "<ctime>",
            "<thread>",
            "<algorithm>",
            "<fstream>",
            "<cstdint>"
            }) {
    }

    void Transpiler::visit(Node* node) {
        switch (node->kind()) {
        case NodeKind::NIL:
            return visit(static_cast<NilNode*>(node));
        case NodeKind::NUMBER:
            return visit(static_cast<NumberNode*>(node));
        case NodeKind::IDENTIFIER:
            return visit(static_cast<IdentifierNode*>(node));
        case NodeKind::TRUE:
            return visit(static_cast<TrueNode*>(node));
        case NodeKind::FALSE:
            return visit(static_cast<FalseNode*>(node));
        case NodeKind::CHAR:
            return visit(static_cast<CharNode*>(node));
        case NodeKind::STRING:
            return visit(static_cast<StringNode*>(node));
        case NodeKind::PAREN_EXPR:
            return visit(static_cast<ParenExprNode*>(node));
        case NodeKind::OBJECT:
            return visit(static_cast<ObjectNode*>(node));
        case NodeKind::VEC:
            return visit(static_cast<VecNode*>(node));
        case NodeKind::LAMBDA_EXPR:
            return visit(static_cast<LambdaExprNode*>(node));
        case NodeKind::CALL_EXPR:
            return visit(static_cast<CallExprNode*>(node));
        case NodeKind::ATTR_EXPR:
            return visit(static_cast<AttrExprNode*>(node));
        case NodeKind::UNARY_EXPR:
            return visit(static_cast<UnaryExprNode*>(node));
        case NodeKind::BINARY_EXPR:
            return visit(static_cast<BinaryExprNode*>(node));
        case NodeKind::EXPR_STMT:
            return visit(static_cast<ExprStmtNode*>(node));
        case NodeKind::VAR_DECL_STMT:
            return visit(static_cast<VarDeclStmtNode*>(node));
        case NodeKind::BLOCK_STMT:
            return visit(static_cast<BlockStmtNode*>(node));
        case NodeKind::IF_STMT:
            return visit(static_cast<IfStmtNode*>(node));
        case NodeKind::IF_ELSE_STMT:
            return visit(static_cast<IfElseStmtNode*>(node));
        case NodeKind::WHILE_STMT:
            return visit(static_cast<WhileStmtNode*>(node));
        case NodeKind::FOR_STMT:
            return visit(static_cast<ForStmtNode*>(node));
        case NodeKind::STMT_LIST:
            return visit(static_cast<StmtListNode*>(node));
        case NodeKind::FUNC_DECL_STMT:
            return visit(static_cast<FuncDeclStmtNode*>(node));
        case NodeKind::RETURN_STMT:
            return visit(static_cast<ReturnStmtNode*>(node));
        case NodeKind::CIMPORT_STMT:
            return visit(static_cast<CImportStmtNode*>(node));
        case NodeKind::TRY_CATCH_STMT:
            return visit(static_cast<TryCatchStmtNode*>(node));
        default:
            throw std::string(filename + ":" + std::to_string(node->line) + ": unknown AST node type");
        }
    }

    void Transpiler::visit(NilNode* node) {
        post_main_code << "TachyonVal::make_nil()";
    }

    void Transpiler::visit(NumberNode* node) {
        post_main_code << "TachyonVal::make_num(";
        post_main_code << node->val;
        post_main_code << ')';
    }

    void Transpiler::visit(TrueNode* node) {
        post_main_code << "TachyonVal::make_bool(true)";
    }

    void Transpiler::visit(FalseNode* node) {
        post_main_code << "TachyonVal::make_bool(false)";
    }

    void Transpiler::visit(CharNode* node) {
        post_main_code << "TachyonVal::make_char('";
        post_main_code << node->val;
        post_main_code << "')";
    }

    void Transpiler::visit(StringNode* node) {
        post_main_code << "TachyonVal::make_str(\"";
        post_main_code << node->val;
        post_main_code << "\")";
    }

    void Transpiler::visit(IdentifierNode* node) {
        post_main_code << node->val;
    }

    void Transpiler::visit(ParenExprNode* node) {
        post_main_code << '(';
        visit(node->node.get());
        post_main_code << ')';
    }
    void Transpiler::visit(LambdaExprNode* node) {
        post_main_code << "TachyonVal::make_func([&](const std::vector<TachyonVal>& args) {\n";
        for (int i = 0; i < node->args.size(); i++) {
            post_main_code << "TachyonVal " << node->args.at(i) << " = args.at(" << i << ");\n";
        }
        if (node->body->kind() == NodeKind::BLOCK_STMT) {
            visit(node->body.get());
            post_main_code << "\nreturn TachyonVal::make_nil();\n})";
        }
        else {
            post_main_code << "return ";
            visit(node->body.get());
            post_main_code << ";\n})";
        }
    }

    void Transpiler::visit(ObjectNode* node) {
        post_main_code << "TachyonVal::make_object({";
        for (int i = 0; i < node->keys.size(); i++) {
            post_main_code << "{\"" << node->keys.at(i) << "\",";
            visit(node->vals.at(i).get());
            post_main_code << '}';
            if (i < node->keys.size() - 1) {
                post_main_code << ',';
            }
        }
        post_main_code << "})";
    }

    void Transpiler::visit(VecNode* node) {
        post_main_code << "TachyonVal::make_vec({";
        for (int i = 0; i < node->elems.size(); i++) {
            visit(node->elems.at(i).get());
            if (i < node->elems.size() - 1) {
                post_main_code << ',';
            }
        }
        post_main_code << "})";
    }

    void Transpiler::visit(CallExprNode* node) {
        visit(node->callee.get());
        post_main_code << "({";
        if (node->callee->kind() == NodeKind::ATTR_EXPR) {
            std::shared_ptr<AttrExprNode> attr_expr_node = std::dynamic_pointer_cast<AttrExprNode>(node->callee);
            visit(attr_expr_node->object.get());
            if (node->args.size()) {
                post_main_code << ',';
            }
        }

        for (int i = 0; i < node->args.size(); i++) {
            visit(node->args.at(i).get());
            if (i < node->args.size() - 1) {
                post_main_code << ',';
            }
        }
        post_main_code << "})";
    }

    void Transpiler::visit(UnaryExprNode* node) {
        post_main_code << '(';
        post_main_code << node->op.val;
        visit(node->node.get());
        post_main_code << ')';
    }

    void Transpiler::visit(BinaryExprNode* node) {
        if (node->node_a->kind() == NodeKind::ATTR_EXPR && node->op.val == "=") {
            std::shared_ptr<AttrExprNode> attr_expr_node = std::dynamic_pointer_cast<AttrExprNode>(node->node_a);
            visit(attr_expr_node->object.get());
            post_main_code << ".o->set(\"" << attr_expr_node->attr << "\",";
            visit(node->node_b.get());
            post_main_code << ')';
        }
        else {
            post_main_code << '(';
            visit(node->node_a.get());
            post_main_code << ' ';
            if (node->op.val == "^^") {
                post_main_code << "!=";
            }
            else {
                post_main_code << node->op.val;
            }
            post_main_code << ' ';
            visit(node->node_b.get());
            post_main_code << ')';
        }
    }

    void Transpiler::visit(ExprStmtNode* node) {
        visit(node->node.get());
        post_main_code << ";";
    }

    void Transpiler::visit(AttrExprNode* node) {
        visit(node->object.get());
        post_main_code << ".o->get(\"" << node->attr << "\")";
    }

    void Transpiler::visit(VarDeclStmtNode* node) {
        post_main_code << "TachyonVal ";
        post_main_code << node->name;
        post_main_code << " = ";
        visit(node->val.get());
        post_main_code << ';';
    }

    void Transpiler::visit(BlockStmtNode* node) {
        post_main_code << "{\n";
        visit(node->node.get());
        post_main_code << "}";
    }

    void Transpiler::visit(IfStmtNode* node) {
        post_main_code << "if((";
        visit(node->test.get());
        post_main_code << ").b) ";
        visit(node->body.get());
    }

    void Transpiler::visit(IfElseStmtNode* node) {
        post_main_code << "if((";
        visit(node->test.get());
        post_main_code << ").b) ";
        visit(node->body.get());
        post_main_code << " else ";
        visit(node->alternate.get());
    }

    void Transpiler::visit(WhileStmtNode* node) {
        post_main_code << "while((";
        visit(node->test.get());
        post_main_code << ").b)";
        visit(node->body.get());
    }

    void Transpiler::visit(ForStmtNode* node) {
        post_main_code << "for(";
        visit(node->init.get());
        post_main_code << ' ';
        post_main_code << "(";
        visit(node->test.get());
        post_main_code << ").b; ";
        visit(node->update.get());
        post_main_code << ") ";
        visit(node->body.get());
    }

    void Transpiler::visit(FuncDeclStmtNode* node) {
        post_main_code << "TachyonVal " << node->name << " = TachyonVal::make_func([&](const std::vector<TachyonVal>& args) {\n";
        for (int i = 0; i < node->args.size(); i++) {
            post_main_code << "TachyonVal " << node->args.at(i) << " = args.at(" << i << ");\n";
        }
        visit(node->body.get());
        post_main_code << "\nreturn TachyonVal::make_nil();\n});";
    }

    void Transpiler::visit(ReturnStmtNode* node) {
        post_main_code << "return ";
        visit(node->node.get());
        post_main_code << ';';
    }

    void Transpiler::visit(CImportStmtNode* node) {
        included_headers.insert("\"" + node->path + "\"");
    }

    void Transpiler::visit(TryCatchStmtNode* node) {
        post_main_code << "try ";
        visit(node->try_body.get());
        post_main_code << "catch(const std::exception& _e) {\nTachyonVal " << node->ex
            << " = TachyonVal::make_object({{\"msg\",TachyonVal::make_str(_e.what())},{\"proto\",Exception}});\n";
        visit(node->catch_body.get());
        post_main_code << "\n}";
    }

    void Transpiler::visit(StmtListNode* node) {
        for (int i = 0; i < node->stmts.size(); i++) {
            std::shared_ptr<Node> stmt = node->stmts.at(i);
            visit(stmt.get());
            post_main_code << '\n';
        }
    }

    std::string Transpiler::generate_code(Node* node) {
        visit(node);
        std::string code = "// Generated by Tachyon\n";
        for (const std::string& header : included_headers) {
            code += "#include " + header + "\n";

        }
        code += boilerplate;
        code += "int main(int argc, char** argv) {\n";
        code += post_main_code.str();
        code += "    return 0;\n}";
        return code;
    }
}; // namespace tachyon