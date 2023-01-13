#include <functional>
#include <string>
#include <vector>

struct val_t;
using func_t = std::function<val_t(const std::vector<val_t>&)};
struct val_t {
    enum { NIL, DOUBLE, BOOL, STRING, FUNC } tag;
    void *ptr; 
    val_t() { tag = NIL; ptr = nullptr; }
    val_t(double d) { tag = DOUBLE; ptr = &d; }
    val_t(bool b) { tag = BOOL; ptr = &b; } 
    val_t(std::string s) { tag = STRING; ptr = &s; }
    val_t(func_t f) { tag = FUNC; ptr = &f; }
    val_t(const val_t& v) {
        tag = v.tag;
        switch(tag) {
        case NIL: {
            break;
        }
        case DOUBLE: {
            double d = *(double *)v.ptr;
            ptr = &d;
            break;
        }
        case BOOL: {
            bool b = *(bool *)v.ptr;
            ptr = &b;
            break;
        }
        case STRING: {
            std::string s = *(std::string *)v.ptr;
            ptr = &s;
            break;
        }
        case FUNC: {
            func_t f = *(func_t *)v.ptr;
            ptr = &f;
            break;
        }
        }
    }
    operator double() const { return *(double *)ptr; }
    operator std::string() const { return *(std::string *)ptr; }
    operator func_t() const { return *(func_t *)ptr;}
};

int main() {
{
val_tx = 5.0;
x;
}
return 0;
}