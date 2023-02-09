#include <cassert>
#include <cmath>

// A copy of this file will be included in every transpiled program

// Tagged union
class ErisVal {
public:
    enum {
        NIL,
        NUM,
        BOOL
    } tag;

    union {
        double n;
        bool b;
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