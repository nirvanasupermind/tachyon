# Tachyon 2.0 Reference Manual
# 1 Introduction
Tachyon is a lightweight dynamic programming language that supports full multithreading and prototype-based object-oriented programming. Tachyon is transpiled to C++11 using tagged unions to handle dynamic typing, and can interact with C++ code.
# 2 Lexical Structure
This section specifies the lexical structure of Tachyon. Extended Backus–Naur form (EBNF) is also provided for the described grammar.

## 2.1 Whitespace
```
whitespace = " " | "\n" | "\t" | "\v" | "\f" | "\r";
```

Whitespace characters include characters that represent horizontal or vertical space, and are ignored by the Tachyon transpiler.

## 2.2 Comments
```
comment = "//", {? any character except for newline ?};
```

A comment is text ignored by the Tachyon transpiler. They are normally used to add explanations to code. A comment is `//` followed by any sequence of characters. Comments are terminated by newlines.
 
```
// This is a comment
```

## 2.3 Literals
```
digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
number = digit, {digit}, [".", digit, {digit}], [("e" | "E"), ["+" | "-"], {digit}];
char = "'", ? any character except for single quote ?, "'"; 
string = '"', {? any character except for double quote ?}, '"'; 
```

Tachyon accepts decimal and scientific numeric literals, character literals and string literals. Character literals are wrapped in single quotes whereas string literals are wrapped in double qoutes. String literals can span multiple lines. Escape sequences in character and string literals are currently not supported in Tachyon, but will be added in a future version.

```
var a = 123;
var b = 64.5;
var c = 1e+5;
var d = 'a';
var e = "String literal";
var f = "Multiline
string literal":
```

## 2.4 Identifiers
```
identifier = (letter | "_"), {letter | digit | "_"};
```

An identifier is an unlimited-length sequence of letters, digits, and underscores which does not start with a digit. Identifiers are case-sensitive.
## 2.5 Keywords
The following keywords are reserved by the language and cannot be used as identifiers:
```
nil true false var block if else while for def lambda return import cimport try catch
```
## 2.6 Operators
The following tokens are the Tachyon operators:
```
+ - * / % == != < > <= >=  && || ! & | ^ << >> ~
```
## 2.7 Other Tokens
Other tokens used are:
```
' " ( ) { } [ ] , : ; . 
```

# 3 Values and Types

## 3.1 Types
Tachyon is a dynamically-typed language. This means that variables themselves do not have a type, although they are to a value that does have a type. All values in Tachyon are first-class values, and can be stored in data structures, variables, and passed and returned from functions.

### 3.1.1 Nil
The nil type represents the absence of a useful value, and has one single value, `nil`.

```
var a = nil;
```
### 3.1.2 Numbers
The number type represents a double-precision (64-bit) floating-point number. Both decimal and scientific notation literals can be used for numbers.
```
var a = 123;
var b = 64.5;
var c = 1e+5;
```

### 3.1.3 Booleans
The boolean type has two values, created using the literals `false` and `true`, which represent the two truth values of logic. 

```
var a = true;
```

### 3.1.4 Characters
The character type represents a single character in text. A character is not a Unicode character but a single byte.

```
var a = 'H';
```

### 3.1.5 Objects
Objects are associative containers implemented as key-value pairs (called a member). Objects can be modified after creation. Objects can inherit members from other objects via their prototype, which is set using the `proto` member. Strings, vectors, and functions are all objects that are derived from the built-in `String`, `Vec` and `Func` objects respectively.

```
var Vec2 = {
    mag: lambda(self) {
        return Math.sqrt(self.x * self.x + self.y * self.y);
    }
};

var vec2 = {x: 3, y: 4, proto: Vec2};
System.print(vec2.mag()); // 5
```

## 3.2 Variables and Scopes
Variables are named bindings to values. Variable declaration statements and function declaration statements can be used to declare variables. The scope of a variable is the part of a program where the variable can be accessed. Scopes use a hierarchical structure with the global scope at the root. A variable can be accesed if it is in the current scope or any of its parent scopes. Block statements create a new child scope of their enclosing scope.

# 4 Expressions
```
expr = assignment expr;
```

This section describes expressions in Tachyon. Extended Backus–Naur form (EBNF) is also provided for the described syntax.

## 4.1 Primary Expressions
```
primary expr = number | char | string | vec expr | object expr | lambda expr | paren expr;
```
Primary expressions include the simplest expressions: [literals](#23-literals), vector expressions, object expressions, lambda expressions, and parenthesized expressions.

## 4.1.1 Vector Expressions
```
expr list = "" | expr, ",", expr list;
vec expr = "[", expr list, "]";
```

Vector expressions can be used used to create [vector](#64-the-vec-object) objects. A vector expression is a list of zero or more comma-seperated expressions enclosed in square brackets.

```
var primes = [2, 3, 5, 7, 11];
```

## 4.1.2 Object Expressions
```
expr list = "" | expr, ",", expr list;
vec expr = "[", expr list, "]";
```

Object expressions can be used used to create [objects](#35-objects). An object expression is a list of zero or more comma-seperated key-value pairs enclosed in curly brackets.


```
var object = {a: 2, b: 5};
```

## 4.1.3 Lambda Expressions
```
arg names = "" | identifier, ",", arg names;
lambda expr = "lamba", "(",  arg names, ")", (expr | block stmt);
```
Lambda expressions can be used used to create anonymous [functions](#55-the-func-object). Lambda expressions have similar syntax to [function declaration statements](#48-function-declaration-statements), with the only major difference being the body of a lambda expression can either be an expression or a block statement.

```
var sqr = lambda(x) x * x;
var add = lambda(a, b) {
    return a + b;
}
```

## 4.1.4 Parenthesized Expressions
```
paren expr = "(", expr, ")";
```
A parenthesized expression is an expression whose value is that of the contained expression.

## 4.2 Call Member Expressions
```
expr list = "" | "expr", ",", expr list;
call member expr = primary expr, {("(", expr list, ")") | (".", identifier)};
```

Call member expressions include function calls, object member accesses, and all combinations of them, in addition to primary expressions. When a function which is a member of an object is called, the first argument will automatically be set to the object, with the expliticly-specified arguments coming after it.
```
System.print("Hello world!");
```
## 4.3 Operators
### 4.3.1 Unary Operators
```
unary expr = {"+" | "-" | "!" | "~"}, call prop expr;
```

The unary operators include `+` (unary plus), `-` (unary minus), `!` (logical NOT), and `~` (bitwise NOT). `+`, `-`, and `~` must take a number as their argument whereas `!` must take a boolean as their argument.

### 4.3.2 Multiplicative Operators
```
multiplicative expr = unary expr | (unary expr, ("*" | "/" | "%"), multiplicative expr);
```

The multiplicative operators are `*` (multiply), `/` (divide), and `%` (modulo or remainder). They are syntatically left-assosciative, must have numbers are arguments, and output numbers. The result of the modulo operator has the same sign as the first argument.

### 4.3.3 Additive Operators
```
additive expr = multiplicative expr | (multiplicative expr, ("+" | "-"), additive expr);
```

The additive operators are `+` (add) and `-` (subtract). They are syntatically left-assosciative, must have numbers as arguments, and output numbers.

## 4.3.4 Shift Operators
```
shift expr = additive expr | (additive expr, ("<<" | ">>"), shift expr);
```

The shift operators are `<<` (right shift) and `>>` (left shift). They are bitwise operators which convert a number to an signed 64-bit integer then shift the bits of the number to the left or right. They are syntatically left-assosciative, must have numbers as arguments, and output numbers. 

## 4.3.5 Comparison Operators
```
comp expr = shift expr | (shift expr, ("<" | ">" | "<=" | ">="), comp expr);
```

The comparison operators are `<` (less than), `>` (greater than), `<=` (less than or equal), and `>=` (greater than or equal). They are syntatically left-assosciative, must have numbers as arguments, and output booleans. 

## 4.3.6 Equality Operators
```
equality expr = comp expr | (comp expr, ("==" | "!="), equality expr);
```

The equality operators are `==` (equal) and `!=` (not equal). They are syntatically left-assosciative, can take any value as arguments, and output booleans. Objects are considered equal if they are the same reference.

## 4.3.7 Bitwise AND Operator
```
bitand expr = equality expr | (equality expr, "&", bitand expr);
```
The bitwise AND (`&`) operator converts both arguments to signed 64-bit integers then takes the logical AND of each of their bits. It is syntatically left-assosciative, must have numbers as arguments, and outputs a number.

## 4.3.8 Bitwise OR Operator
```
bitand expr = equality expr | (equality expr, "&", bitand expr);
```
The bitwise OR (`|`) operator converts both arguments to signed 64-bit integers then takes the logical OR of each of their bits. It is syntatically left-assosciative, must have numbers as arguments, and outputs a number.

## 4.3.9 Bitwise XOR Operator
```
bitxor expr = bitand expr | (bitand expr, "^", bitxor expr);
```
The bitwise XOR (`^`) operator converts both arguments to signed 64-bit integers then takes the logical XOR of each of their bits. It is syntatically left-assosciative, must have numbers as arguments, and outputs a number.

## 4.3.10 Bitwise OR Operator
```
bitor expr = bitxor expr | (bitxor expr, "|", bitor expr);
```
The bitwise OR (`|`) operator converts both arguments to signed 64-bit integers then takes the logical OR of each of their bits. It is syntatically left-assosciative, must have numbers as arguments, and outputs a number.

## 4.3.11 Loigcal AND Operator
```
and expr = bitor expr | (and expr, "&&", bitor expr);
```
The logical AND (`&&`) operator takes the logical AND of two booleans. It is syntatically left-assosciative, must have booleans as arguments, and outputs a boolean.

## 4.3.12 Loigcal OR Operator
```
and expr = bitor expr | (and expr, "&&", bitor expr);
```
The logical OR (`||`) operator takes the logical OR of two booleans. It is syntatically left-assosciative, must have booleans as arguments, and outputs a boolean. There is no logical XOR operator as it would be identical to `!=` for boolean arguments. 

## 4.3.13 Loigcal OR Operator
```
or expr = and expr | (and expr, "||", or expr);
```
The logical OR (`||`) operator takes the logical OR of two booleans. It is syntatically left-assosciative, must have booleans as arguments, and outputs a boolean.

# 5 Statements
```
stmt = expr stmt | var decl stmt | block stmt | if stmt | while stmt | for stmt | func decl stmt; 
```

This section describes statements in Tachyon. Extended Backus–Naur form (EBNF) is also provided for the described syntax.

## 5.1 Expression Statements
```
expr stmt = expr, ";";
```
Expressions may be used as statements by following them with semicolons. The value of the expression is discarded.

## 5.2 Variable Declaration Statements
```
var decl stmt = "var", identifier, "=", expr, ";";
```

A variable declaration statement declares a variable name.

```
var a = 5;
```

## 5.3 Variable Declaration Statements
```
var decl stmt = "var", identifier, "=", expr, ";";
```

A variable declaration statement declares a variable name.

```
var a = 5;
```

## 5.4 Block Statements
```
block stmt = "{", {stmt}, "}";
```

A block is a sequence of statements within braces. Block statements create a new child scope of their enclosing scope.

```
{
    var x = 2;
}
// x cannot be accessed from outside
```

## 5.5 If Statements
```
if stmt = "if", "(", expr, ")", block stmt, ["else", block stmt];
```

The if statement executes a statement if the condition is `true`. If the condition is `false`, another statement in the optional else clause will be executed. The condition of an if-else statement must be a boolean.

```
if(a < 5) {
    a = 0;
} else {
    a = 1;
}
```

## 5.7 While Statements
```
while stmt = "while", "(", expr, ")", block stmt;
```

The while statement executes a statement while the condition is true.

```
while(y < 10) {
    x = x + 1;
}
```

## 5.8 For Statements
```
for stmt = "for", "(", var decl stmt, expr, ";", expr, ")", block stmt;
```

The for statement executes some initialization code, then executes an expression, a statement, and some update code repeatedly until the value of the expression is false.

```
for (var i = 0; i < 10; i = i + 1) {
    System.print(i);
}
```

## 5.9 Function Declaration Statements
```
func decl stmt = "def", identifier, "(", arg names, ")", block stmt;
```

A function declaration statements declares a named function.

```
def sqr(x) {
    return x * x;
}
```

## 5.10 Import Statements
```
import stmt = "import", string, ";";
```

An import statement is used to include another Tachyon file.

```
import "file.tachyon";
```

## 5.11 C++ Import Statements
```
cpp import stmt = "cppimport", string, ";";
```
A C++ import statement is used to include the contents of another C++ file inside the C++ code emitted by the transpiler. C++ import statements can be used to allow interop with C++.

```
cppimport "file.cpp";
```


## 5.12 Try-Catch Statements
The try-catch statement is used for runtime exception handling. If a runtime exception is thrown during the execution of the `try` clause, the `catch` clause will be used as a fallback.

```
try {
    [1, 2, 3].at(3);
} catch(e) {
    System.print("Error message: ".concat(e.msg));
}
```

# 6 The Standard Library
## 6.1 The System Object
The `System` object exposes operating system facilities like I/O, date time manipulation etc.

### Members
#### `System.print(self, a)`
Prints a value to the console followed by a newline.
#### `System.input(self)`
Returns the user input as a string.
#### `System.exit(self)`
Terminates the program.
#### `System.time(self)`
Returns the current Unix epoch in milliseconds.


## 6.2 The Math Object
The `Math` object contains commonly-used mathematical constants and routines.

### Members
#### `Math.PI`
The closest float64 approximation of the mathematical constant pi.
#### `Math.E`
The closest float64 approximation of the mathematical constant e.
#### `Math.sin(self, a)`
Returns the sine of `a`.
#### `Math.cos(self, a)`
Returns the cosine of `a`.
#### `Math.tan(self, a)`
Returns the tangent of `a`.
#### `Math.asin(self, a)`
Returns the inverse sine of `a`.
#### `Math.acos(self, a)`
Returns the inverse cosine of `a`.
#### `Math.atan(self, a)`
Returns the inverse tangent of `a`.
#### `Math.atan2(self, a, b)`
Returns the inverse tangent of `a`/`b`.
#### `Math.exp(self, a)`
Returns the exponential of `a`.
#### `Math.log(self, a)`
Returns the natural logarithm of `a`.
#### `Math.sqrt(self, a)`
Returns the square root of `a`.
#### `Math.pow(self, a)`
Returns `a` raised to the power of `b`.
#### `Math.ceil(self, a)`
Returns the ceiling function of `a`.


## 6.3 The String Object
The `String` object represents ordered sequences of [characters](#3.1.4-characters). All strings  are implemented as objects whose prototype is `String`. The characters inside a string are assigned a zero-based index number, with the first character being indexed 0, the second being indexed 1, the third being indexed 2, and so on.

### Members
#### `String.length(self)`
R#eturns the length of `self`.
#### `String.at(self, idx)`
Returns the character indexed `idx` in `self`.
#### `String.first(self)`
Returns the first character in `self`.
#### `String.last(self)`
Returns the last character in `self`.
#### `String.find(self, str)`
Returns the index of the first instance of `str` in `self`.
#### `String.contains(self, str)`
Determines whether `self` contains `str`.
#### `String.substr(self, pos, len)`
Returns the substring of `self` starting at index `pos` and having a length of `len`.
#### `String.concat(self, str)`
Returns the concatenation of `self` and `str`.
#### `String.split(self, sep)`
Splits the string into a vector of substrings using a specified seperator.
#### `String.from(self, val)`
Returns a string representation of the specified value. Objects aside from strings themselves are represented by the string "&lt;object&gt;".

## 6.4 The Vec Object
The `Vec` object represents ordered sequences of any value. A single vector can contain values of different types. All vectors are implemented as objects whose prototype is `Vec`. The elements inside a vector are assigned a zero-based index number, with the first element being indexed 0, the second being indexed 1, the third being indexed 2, and so on.

### Members
#### `Vec.length(self)`
Returns the length of `self`.
#### `Vec.at(self, idx)`
Returns the element indexed `idx` in `self`.
#### `Vec.first(self)`
Returns the first element in `self`.
#### `Vec.last(self)`
Returns the last element in `self`.
#### `Vec.add(self, el)`
Adds the element `el` to the vector.
#### `Vec.remove(self, idx)`
Removes the element at index `idx` from the vector.
####  `Vec.subvec(self, pos, len)`
Returns the subector of `self` starting at index `pos` and having a length of `len`.

## 6.5 The Func Object
The `Func` object represents a block of code which only runs when it is called. Data known as arguments can be passed to a function. The first argument of a function which is a member of an object, named `self` by convention, will be impliticly set to the object, with expliticitly-stated argumets being placed after it.

## 6.6 The Thread Object
The `Thread`  object represents a block of code that can be executed concurrently with other such blocks in multithreading environments.
### `Thread.create(self, run)`
Returns a thread that will run the function `run` when starting.
### `Thread.start(self)`
Causes the thread `self` to start execution.

## 6.7 The FileSystem Object
The `FileSystem` object contains functions for file I/O.
### Members
#### `FileSystem.read(self, path)`
R#eturns the content of `path`.
#### `FileSystem.write(self, path, str)`
Writes `str` to path.
## 6.8 The Exception Object
The `Exception` object represents runtime exceptions.
### Members
#### `msg`
Contains the message of the exception.
#### `Exception.throw(self)`
Throws `self`.

# 7 The C++ API
## 7.1 The TachyonVal Class
The `TachyonVal` class is a tagged union type that can represent all Tachyon values. It consists of an untagged union and a tag field. It can
#### Properties
#### `tag`
The tag of the value. Can be `NIL`, `NUM`, `BOOL`, `CHAR`, or `OBJECT`.
#### `double n`
Part of the untagged union, represents the C++ value of a Tachyon number. 
#### `bool b`
Part of the untagged union, represents the C++ value of a Tachyon boolean. 
#### `TachyonObject* o`
Part of the untagged union, represents the C++ value of a Tachyon object. 
#### Methods
#### `static TachyonVal make_nil();`
Returns `nil`.
#### `static TachyonVal make_num(double n)`
Returns a Tachyon number with the value of `n`.
#### `static TachyonVal make_bool(bool b)`
Returns a Tachyon boolean with the value of `b`.
#### `static TachyonVal make_object(bool b)`
Returns a Tachyon boolean with the value of `b`.


## 7.2 The TachyonObject Class
The `TachyonObject` class represents an object in Tachyon. It is wrapper around an `std::map`.

### 7


## 7.3 The TachyonString Class
## 7.4 The TachyonVector Class
## 7.5 The TachyonFunc Class
## 7.6 Using the Tachyon Standard Library