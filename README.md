# Eris

**This interpreter is in a very early stage. Even basic programs will not run.**

C++ interpreter for the Eris programming language. Based on jackprogramsjp's [simple math interpreter](https://github.com/jackprogramsjp/CPP-Simple-Math-Interpreter-V2).

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

print(p);
```