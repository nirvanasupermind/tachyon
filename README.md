![Eris](logo.png)

**Version 2.0.0-dev**

Eris is a lightweight dynamic programming language that supports full multithreading and prototype-based object-oriented programming. Eris is transpiled to C++11 using tagged unions to handle dynamic typing.


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