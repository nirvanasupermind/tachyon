# Eris

**This interpreter is in an early stage. Some programs (including the example below) will not run yet.**

C++ interpreter for the Eris programming language with a header-only implementation. 

# Synopsis
```
class Point {
    def constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    def calc() {
        return this.x + this.y;
    }
}

let p = new Point(10, 20);

print(p.calc()); // 30
```
