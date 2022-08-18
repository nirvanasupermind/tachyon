# Eris 
**Version 1.0.0**

C++ interpreter for the Eris programming language with a header-only implementation.

Eris is a general-purpose, lightweight, dynamic programming language with class-based object-oriented programming support.

# Basic example
```
class Point {
    def constructor(self, x, y) {
        self.x = x;
        self.y = y;
    }

    def calc(self) {
        return Math::pow(self.x, 2.0) + Math::pow(self.y, 2.0);
    }
}

let p = Point(5, 6);
print(p.calc()); // 61.0
```

# Documentation
Currently, documentation is sparse, however, there are:
* [Example programs](examples) 
* [List of all STL classes and functions](stdlib.txt)