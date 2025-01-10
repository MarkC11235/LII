# **CastleLang** 

## **Intro**

CastleLang is a dynamically typed, interpreted language. Everything is passed by value. The language is designed to be easy to use and read. 
The Castle Vitual Machine runs .cl_exe files. These files are compiled from .cl files which are written in CastleLang.


To Use:

1. Clone this repository
2. Navigate to the folder
3. Run the command 'make build_bytecode' which will output the lii executable file
4. OPTIONAL: add the excutable file to your path to use it from anywhere
5. Run the command 'lii your-file.cl'


## **Syntax Highlighting for Visual Studio Code**

https://marketplace.visualstudio.com/items?itemName=MarkCastle.cl  

Extension Name: CastleLang Theme

## **Values**

Types of Values

1. Number (holds any type of number, ex: 2, -43, 1.45)
2. String (holds any characters that appear between quotation marks)
3. Boolean (holds true or false)
4. Null (holds nothing)
5. Vector (holds a list of Values, can be a mix of types)
6. Function (holds a function definition)
7. Struct (holds a list of key value pairs)

## **Functions**

All functions must return a value. This also means that when calling a function, you must assign the return value to a variable.

## **Syntax Examples**

### Assignment and printing
```
let name = "Bob";
let age = 25;

print name + " is " + age + " years old"; // Output: Bob is 25 years old
```

### Vectors
```
let vec = [1, 2, 3, 4, 5];
print vec[2]; // Output: 3
```

### Maps
```
let Person = {
    "name" : "Bob",
    "age" : 25
};

print Person["name"]; // Output: Bob
print Person["age"]; // Output: 25
```

### Functions
```
let factorial = func(n){
    if(n == 0){
        return 1; 
    }
    return n * factorial(n - 1);
};

print factorial(5); // Output: 120
```

### Control flow
#### For and if
```
for(let i = 1; i <= 10; i = i + 1){
    print i;
    if(i == 5){
        print "Halfway";
    }
}

// Output:
// 1
// 2
// 3
// 4
// 5
// Halfway
// 6
// 7
// 8
// 9
// 10
```
#### Break and continue
```
for(let i = 1; i <= 6; i = i + 1){
    if(i == 3){
        continue;
    }
    if(i == 5){
        break;
    }
    print i;
}

// Output:
// 1
// 2
// 4
```
#### If and Else
```
let x = 5;
if(x == 5){
    print "x is 5";
}
else{
    print "x is not 5";
}

// Output: x is 5
```

```
let x = 6;
if(x == 5){
    print "x is 5";
}
else{
    print "x is not 5";
}

// Output: x is not 5
```


### Standard Library Call
```
let str = "Hello World!";
print $string_len(str); // Output: 12
```

## **Standard Library** 

CastleLang has a built in standard library implemented in C++. These functions are able to be called from .cl file by prefixing a function call with '$'. An example of this is -> 
```
let str = "Hello World!";
print $string_len(str);
```

The VM has a foreign function interface (FFI) that allows CastleLang to call predefined C++ functions. The interface is a list that contains information about each function,
1. name of function
2. function pointer
3. return type
4. list of parameter types 

The function pointer is created dynamically using templates and lambda functions, so that functions with any return type and parameter types can be used without modifying the interface. 

To add more functions, add another entry to the STD_LIB_FUNCTIONS_DEFINITIONS vector with the proper function information.

## **Process to compile and run CastleLang**

There are four steps to compile and run CastleLang. 
1. Tokenization  
The Tokenizer takes in a file with the .cast file extension and will go through the file line by line creating tokens that represent different parts of the language. Tokens can be keywords, different symbols like semicolons, identifiers, numbers, etc..
    
2. Parsing  
The Parser takes in a list of tokens and creates an abstract syntax tree using recursive decent parsing.

3. Bytecode Generation  
The Bytecode Generator takes in an abstract syntax tree and generates 
    1. bytecode
    2. constants array
        - Numbers, Strings, Functions, null, and bools
    3. variable names array

4. Virtual Machine  
The Virtual Machine takes in the bytecode generator output and steps through the bytecode array. The VM uses a stack to evaluate expressions and call functions.

## **Debugger**

Can run the bytecode in debug mode using the -d flag. This allows you to step through the bytecode one instruction at a time and after each instruction the debugger will print useful information.


## **Known Issues**

Cannot print functions inside of a JITed function.  
Error when running the test:
symbol lookup error: ./jit_functions/jit_0.so: undefined symbol: _Z16display_bytecodeP8function
Test tests_2/types/test_all_types_as_passable.cl Failed  

No checking for the proper number of arguments when calling a function.  


