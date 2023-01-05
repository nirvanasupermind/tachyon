![Eris](logo.png)

**Version 2.0.0-dev**

Eris is a fast, lightweight dynamic programming language that supports multithreading and class-based object-oriented programming. Eris transpiles to C++17, making it highly performant compared to most other dynamic languages that rely on an interpreter or bytecode VM. Eris supports full multithreading, unlike other languages that have global interpreter locks or use non-thread coroutines.

# Example
```
class Vec2 {
    func Vec2(x, y) {
        this.x = x;
        this.y = y;
    }

    func mag() {
        return eris::sqrt(this.x * this.x + this.y * this.y);
    }
}

var vec2 = Vec2(3, 4);
eris::print(vec2.mag()); // 5
```

```

