#include <string>
#include <map>

enum class ValType {
    INT,
    DOUBLE,
    BOOL,
    OBJECT
};

class Val {
    virtual ValType type() const = 0;
};

class Int: public Val {
public:
    int val;

    Int(int val)
        : val(val) {
    }

    operator int() const {
        return val;
    }

    ValType type() const {
        return ValType::INT;
    }
};

class Double: public Val {
public:
    double val;

    Double(double val)
        : val(val) {
    }

    operator double() const {
        return val;
    }

    ValType type() const {
        return ValType::DOUBLE;
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
};

class Object: public Val {
public:
    std::map<std::string, Val *> props{};

    Object() = default;

    Val *get(const std::string& prop) {
        if (!props.count(prop) && props.count("proto")) {
            Object *proto = static_cast<Object *>(props.at("proto"));
            return proto->get(prop);
        }

        return props.at(prop);
    }

    void set(const std::string& prop, Object *val) {
        props[prop] = val;
    }
};