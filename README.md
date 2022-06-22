# Eris

**This interpreter is in an early stage.**

C++ reference interpreter for the Eris programming language with a header-only implementation.
The specification and example programs for Eris can also be accessed on this repo on [#SPEC.md].

Lua is implemented as a library, written in clean C, the common subset of Standard C and C++. The Lua distribution includes a host program called lua, which uses the Lua library to offer a complete, standalone Lua interpreter, for interactive or batch use. 

# Synopsis
```
class Point {
    def constructor(self, x, y) {
        self.x = x;
        self.y = y;
    }

    def calc(self) {
        return self.x + self.y;
    }
}

let p = Point(10, 20);
print p.calc(); // 30
```
