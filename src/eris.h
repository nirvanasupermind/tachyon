#ifndef ERIS_H
#define ERIS_H

#include <cassert>
#include <cmath>
#include <functional>
#include <string>
#include <vector>
#include <map>

// A copy of this file will be included in every transpiled program

class ErisObject {
public:
    std::map<std::string, ErisVal> map{};

    ErisObject() = default;

    ErisObject(std::map<std::string, ErisVal> map)
        : map(map) {
    }

    ErisVal get(const std::string& key) const {
        if(!(map.count(key)) && map.count("proto")) {
            return map.at("proto").o->get(key);
        } else {
            return map.at(key);
        }
    }
    
    void set(const std::string& key, const ErisVal& val) {
        map[key] = val;
    }
};

class ErisFunc : public ErisObject {
public:
    std::function<ErisVal(std::vector<ErisVal>)> f;
    ErisFunc(const std::function<ErisVal(std::vector<ErisVal>)>& f)
        : f(f) {

    }
};

// Tagged union
class ErisVal {
public:
    enum {
        NIL,
        NUM,
        BOOL,
        CHAR,
        FUNC,
        OBJECT
    } tag;

    union {
        double n;
        bool b;
        char c;
        ErisObject* o;
    };


    static ErisVal make_nil() {
        ErisVal result;
        result.tag = NIL;
        return result;
    }

    static ErisVal make_num(double n) {
        ErisVal result;
        result.tag = NUM;
        result.n = n;
        return result;
    }

    static ErisVal make_bool(bool b) {
        ErisVal result;
        result.tag = BOOL;
        result.b = b;
        return result;
    }

    static ErisVal make_char(char c) {
        ErisVal result;
        result.tag = CHAR;
        result.c = c;
        return result;
    }

    static ErisVal make_func(const std::function<ErisVal(std::vector<ErisVal>)>& f) {
        ErisVal result;
        result.tag = FUNC;
        result.o = new ErisFunc(f);
        return result;
    }

    ErisVal operator+() const {
        assert(tag == NUM);
        return ErisVal::make_num(+n);
    }

    ErisVal operator-() const {
        assert(tag == NUM);
        return ErisVal::make_num(-n);
    }

    ErisVal operator+(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num(n + other.n);
    }

    ErisVal operator-(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num(n - other.n);
    }

    ErisVal operator*(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num(n * other.n);
    }

    ErisVal operator/(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num(n / other.n);
    }

    ErisVal operator%(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num(std::fmod(n, other.n));
    }

    ErisVal operator<<(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num((int)n << (int)other.n);
    }

    ErisVal operator>>(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num((int)n >> (int)other.n);
    }

    ErisVal operator&(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num((int)n & (int)other.n);
    }

    ErisVal operator|(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num((int)n | (int)other.n);
    }

    ErisVal operator^(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_num((int)n ^ (int)other.n);
    }

    ErisVal operator&&(const ErisVal& other) const {
        assert(tag == BOOL && other.tag == BOOL);
        return ErisVal::make_bool(n && other.n);
    }

    ErisVal operator||(const ErisVal& other) const {
        assert(tag == BOOL && other.tag == BOOL);
        return ErisVal::make_bool(n || other.n);
    }

    ErisVal operator<(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_bool(n < other.n);
    }

    ErisVal operator<=(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_bool(n <= other.n);
    }

    ErisVal operator>(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_bool(n > other.n);
    }

    ErisVal operator>=(const ErisVal& other) const {
        assert(tag == NUM && other.tag == NUM);
        return ErisVal::make_bool(n >= other.n);
    }

    ErisVal operator==(const ErisVal& other) const {
        if (tag == NIL) {
            return ErisVal::make_bool(other.tag == NIL);
        }
        else if (tag == NUM) {
            return ErisVal::make_bool(other.tag == NUM && n == other.n);
        }
        else if (tag == BOOL) {
            return ErisVal::make_bool(other.tag == BOOL && b == other.b);
        }
    }

    ErisVal operator!=(const ErisVal& other) const {
        return ErisVal::make_bool(!(operator==(other)).b);
    }
};

#endif // ERIS_Hs