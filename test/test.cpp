#include <cstdio>
#include <cmath>
#include <functional>
#include <string>
#include <vector>

struct val_t;
using func_t = std::function<val_t(const std::vector<val_t>&)>;
struct val_t {
    enum { NIL, NUMBER, BOOL, STRING, FUNC } tag;
    void *ptr; 
    val_t() { tag = NIL; ptr = nullptr; }
    val_t(double d) {
        tag = NUMBER;
        ptr = malloc(sizeof(double));
        *(double *)ptr = d;
    }
    val_t(bool b) {
        tag = BOOL;
        ptr = malloc(sizeof(bool));
        *(bool *)ptr = b;
    }
    val_t(const std::string& s) {
        tag = STRING;
        ptr = malloc(sizeof(std::string));
        *(std::string *)ptr = s;
    }
    val_t(const func_t& f) {
        tag = FUNC;
        ptr = malloc(sizeof(func_t));
        *(func_t *)ptr = f;
    }
    val_t(const val_t& v) {
        tag = v.tag;
        switch(tag) {
        case NIL: {
            break;
        }
        case NUMBER: {
            double d = *(double *)v.ptr;
            ptr = malloc(sizeof(double));
            *(double *)ptr = d;
            break;
        }
        case BOOL: {
            bool b = *(bool *)v.ptr;
            ptr = malloc(sizeof(bool));
            *(bool *)ptr = b;
            break;
        }
        case STRING: {
            std::string s = *(std::string *)v.ptr;
            ptr = malloc(sizeof(std::string));
            *(std::string *)ptr = s;
            ptr = &s;
            break;
        }
        case FUNC: {
            func_t f = *(func_t *)v.ptr;
            ptr = malloc(sizeof(func_t));
            *(func_t *)ptr = f;
            break;
        }
        }
    }
    ~val_t() {
    }
    operator double() const { return *(double *)ptr; }
    operator std::string() const { return *(std::string *)ptr; }
    operator func_t() const { return *(func_t *)ptr;}
};

int main() {
val_t printf = (func_t)([](const std::vector<val_t>& params) -> val_t {
    std::printf("%s", ((std::string)(params.at(0))).c_str());
    return val_t();
});
val_t sin = (func_t)([](const std::vector<val_t>& params) -> val_t {
    return std::sin(params.at(0));
});
((func_t)printf)({((func_t)sin)({(double)3.14})});
return 0;
}