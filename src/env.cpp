#include <string>
#include <memory>
#include <map>
#include <stdexcept>

#include "value.h"
#include "env.h"

namespace eris {
    Env::Env()
        : parent(nullptr) {
    }

    Env::Env(Env* parent)
        : parent(parent) {
    }

    Env* Env::find_env(const std::string& name) {
        if (record.count(name)) {
            return this;
        }
        else if (parent) {
            return parent->find_env(name);
        }
        else {
            throw std::exception();
        }
    }

    Value Env::get(const std::string& name) {
        return find_env(name)->record.at(name);
    }

    void Env::set(const std::string& name, const Value& val) {
        if(!record.count(name)) {
            throw std::exception();
        }

        record[name] = val;
    }

    void Env::def(const std::string& name, const Value& val) {
        if(record.count(name)) {
            throw std::exception();
        }

        record[name] = val;
    }
} // namespace eris
