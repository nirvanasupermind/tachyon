# 1 Introduction
Eris is a lightweight dynamic programming language that supports full multithreading and prototype-based object-oriented programming. Eris is transpiled to C++11 using tagged unions to handle dynamic typing.

# 2 Formal Grammar
## 2.1 Lexical Grammar
## 2.2 Syntactic Grammar
# 3 Values and Types
## 3.1 Nil
The nil type represents the absence of a useful value, and has one single value, which can be created using the literal `nil`.

## 3.2 Numbers
The number type represents a double-precision (64-bit) floating-point number. Numbers can be created using a numeric literal, which can be either a decimal number such as `64.5` or a scientific notation number such as `6.45e+1`.

## 3.3 Booleans
The boolean type has two values, created using the literals `false` and `true`, which represent the two truth values of logic. 

## 3.4 Characters
The character type represents a single character in text. Note that characters in Eris correspond to bytes rather than Unicode characters, in order to be compatible with C++. Literal characters are delimited by single quotess such a. `Z`'.

## 3.5 Objects
The object type represents a map of keys to values that can be inherited from using a prototype-based object model. The key-value pairs contained inside an object are known as properties. Objects can inherit properties directly from other objects through their `proto` property. Objects can be created using JSON-like literals where keys and values are separated by a colon such as `{a; 2, b: 3}`. There is no root of the object hierarchy in Eris and some objects can have no prototype.

## 3.5.1 Strings 
## 3.5.2 Vectors
## 3.5.3 Functions
# 4 Expressions
## 4.1 Lambda Expressions
## 4.2 Function Calls
## 4.3 Unary Operators
## 4.4 Arithmetic Operators
## 4.5 Bitwise Operators
## 4.6 Comparison Operators
## 4.7 Equality Operators
## 4.8 Logical Operators
## 4.9 Assignment Operators
# 5 Statements
## 5.1 Variable Declaration Statements
## 5.2 Block Statements
## 5.3 If Statements
## 5.4 If-Else Statements
## 5.5 While Statements
## 5.6 For Statements
## 5.7 Function Declaration Statements
## 5.8 Import Statements
## 5.9 C++ Import Statements
## 6.0 Try-Catch Statements

# 6 The Standard Library
## 6.1 The System Object
## 6.2 The Math Object
## 6.3 The String Object
## 6.4 The Vec Object
## 6.5 The Func Object
## 6.6 The Thread Object
# 7 The C++ API
## 7.1 The ErisVal Class
## 7.2 The ErisObject Class
## 7.3 The ErisString Class
## 7.4 The ErisVector Class
## 7.5 The ErisFunc Class
## 7.6 Using the Eris Standard Library