#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>

enum class ValType {
    NIL,
    NUMBER,
    BOOL,
    OBJECT
};

class Val {
public:
    virtual ValType type() const = 0;
    virtual std::string str() const = 0;
};

class Nil: public Val {
public:
    Nil() = default;

    ValType type() const {
        return ValType::NIL;
    }

    std::string str() const {
        return "nil";
    }
};

class Number: public Val {
public:
    double val;

    Number(double val)
        : val(val) {
    }

    operator double() const {
        return val;
    }

    ValType type() const {
        return ValType::NUMBER;
    }

    std::string str() const {
        return std::to_string(val);
    }
};

class Bool: public Val {
public:
    bool val;

    Bool(bool val)
        : val(val) {
    }

    operator bool() const {
        return val;
    }

    ValType type() const {
        return ValType::BOOL;
    }

    std::string str() const {
        return val ? "true" : "false";
    }
};

class Object: public Val {
public:
    std::map<std::string, Val*> props{};

    Object() = default;

    Val* get(const std::string& prop) {
        if (!props.count(prop) && props.count("proto")) {
            Object* proto = static_cast<Object*>(props.at("proto"));
            return proto->get(prop);
        }

        return props.at(prop);
    }

    void set(const std::string& prop, Val* val) {
        props[prop] = val;
    }

    ValType type() const {
        return ValType::OBJECT;
    }

    std::string str() const {
        std::ostringstream ss;
        ss << "object: " << this;
        return ss.str();
    }
};

class StringObject: public Object {
public:
    std::string val;

    StringObject(const std::string& val)
        : val(val) {
        set("proto", String);
    }

    std::string str() const {
        return val;
    }
};

class VectorObject: public Object {
public:
    std::vector<Val*> val;

    VectorObject(const std::vector<Val*>& val)
        : val(val) {
        set("proto", Vector);
    }
};

class FuncObject: public Object {
public:
    std::function<Val* (const std::vector<Val*>&)> val;

    FuncObject(const std::function<Val* (const std::vector<Val*>&)>& val)
        : val(val) {
        set("proto", Func);
    }
};

Val* String = new Object();
Val* Vector = new Object();
Val* Func = new Object();
Val* System = new Object();

void eris_init() {
    static_cast<Object*>(System)->set("print", new FuncObject([](const std::vector<Val*>& params) {
        std::cout << params.at(0)->str() << '\n';
    return new Nil();
        }));
}