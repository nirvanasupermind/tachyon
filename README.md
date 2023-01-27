![Eris](logo.png)

**Version 2.0.0-dev**

Eris is a fast, lightweight dynamic programming language that supports multithreading and prototype-based object-oriented programming. Eris transpiles to C++11, making it highly performant compared to most other dynamic languages that rely on an interpreter or bytecode VM. Eris supports full multithreading, unlike other languages that have global interpreter locks or use non-thread coroutines.

# Example
```
var Vec2 = {
    mag: lambda() {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }
};

var vec2 = {x: 3, y: 4, proto: Vec2};
System.print(vec2.mag()); // 5
```

```

