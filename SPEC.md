<!-- # Introduction -->
# 1 - Grammar
## 1.1 - Microsyntax
## 1.2 - Macrosyntax
# 2 - Values and Data Types
Eris is a dynamically typed language, so ariables do not have types; only values do. In Eris, every value is first-class. This implies that any values can be used as parameters for other functions, saved in variables, and returned as results.
## 2.1 - Null
The null type has only one value created using the `null` literal.
```
let a = null;
```
## 2.2 - Integer
The integer type represents a signed 2's complement 32-bit integer.
```
let a = 123;
let b = 4567;
```
## 2.3 - Double
The double type represents a IEEE-754 double-precision floating-point number.
```
let a = 1.0;
let b = 0.234;
```
## 2.4 - Boolean
The boolean type has two values created using the `true` and `false` literals.
```
let a = true;
```
## 2.5 - Char
The character type represents a single byte in a `String` object.
```
let a = 'w';
```
## 2.6 - Object
The object type represents an instance of a class (see [8 - Classes and Objects](#8---Classes-and-Objects)). There are several specialized objects, namely instances of `String`, `List`, and `Func`, which support additional functionality, such as storing a sequence of characters in the case of `String` instances.
```
let a = Object();
```
## 2.7 - Class
The class type represents a class that can construct objects (see [8 - Classes and Objects](#8---Classes-and-Objects)). 
```
class a {
}
```
## 2.8 - Namespace
The class type represents a namespace that can contain scoped variables and functions (see [9 - Namespaces](#9 - Namespaces)).
```
namespace a {
}
```
# 3 - Execution Context
# 4 - Statements
## 4.1 - Declaration Statements
### 4.1.1 - Variable Declaration Statements
### 4.1.2 - Function Declaration Statements
### 4.1.3 - Class Declaration Statements
### 4.1.4 - Namespace Declaration Statements
## 4.2 - If Statements
## 4.3 - For Statements
## 4.4 - While Statements
# 5 - Expressions
## 5.1 - Assignment
## 5.2 - Operators
### 5.2.1 - Arithmetic Operators
### 5.2.2 - Logical Operators
### 5.2.3 - Relational Operators
### 5.2.4 - Equality Operators
## 5.3 - Literals
### 5.3.1 - Integer Literals
### 5.3.2 - Double Literals
### 5.3.3 - Boolean Literals
### 5.3.4 - Char Literals
### 5.3.5 - String Literals
### 5.3.6 - List Literals
## 7 - Functions
## 8 - Classes and Objects
## 9 - Namespaces
## 10 - Imports
## 11 - The Standard Library
### 11.1 - Global Functions
### 11.2 - The Class `Object`
### 11.3 - The Class `String`
### 11.4 - The Class `List`
### 11.5 - The Class `Func`
### 11.6 - The Class `Math`

<!-- ## 11 - Strings
## 7 - Collections
## 8 - Math Functions
## 9 - Utility Functions -->