# Eris

**This interpreter is in a very early stage. Even basic programs will not run.**

C++ interpreter for the Eris programming language. Based on jackprogramsjp's [simple math interpreter](https://github.com/jackprogramsjp/CPP-Simple-Math-Interpreter-V2).

# Compilaton

To compile on Unix, navigate to the Eris directory and run

```
source build.sh
```

An executable named `eris` will be created.

<hr />

To compile on Windows, navigate to the Eris directory and run

```
build.cmd
```

An executable named `eris.exe` will be created.

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

var p = Point(10, 20);

print(p.calc());
```

