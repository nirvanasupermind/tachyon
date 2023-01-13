#include <string>
#include <iostream>
#include <set>
#include <memory>

#include "error.h"
#include "env.h"

namespace eris {
    Env::Env()
        : scope(std::set<std::string>{}), parent(nullptr) {
    }

    Env::Env(std::shared_ptr<Env> parent)
        : scope(std::set<std::string>{}), parent(parent) {
    }

    void Env::check_name(const std::string& name, const std::string& filename, size_t line) {
        if((!scope.empty()) && scope.count(name)) {
            return;
        } else if(parent) {
            parent->check_name(name, filename, line);
        } else {
            raise_error(filename, line, name + " is not defined");
        }
    }

    void Env::add_name(const std::string& name) {
        scope.insert(name);
    }
} // namespace eris