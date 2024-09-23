# Tachyon

**Version 2.0.0**

Tachyon is a lightweight dynamic programming language that supports full multithreading and prototype-based object-oriented programming. Eris is transpiled to C++11 using tagged unions to handle dynamic typing, and can interact with C++ code.


Reference document: [REFERENCE.MD]
# Example
```
var Vec2 = {
    mag: lambda(self) {
        return Math.sqrt(self.x * self.x + self.y * self.y);
    }
};

var vec2 = {x: 3, y: 4, proto: Vec2};
System.print(vec2.mag()); // 5
```

# Changelog
* 1.0.0