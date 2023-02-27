![Eris](logo.png)

**Version 2.0.0-dev**

Eris is a fast, lightweight dynamic programming language that supports full multithreading and prototype-based object-oriented programming. Eris transpiles to C++11, making it highly performant compared to other dynamic languages that rely on an interpreter or bytecode VM.


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