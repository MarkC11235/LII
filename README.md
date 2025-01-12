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

### **Requirements**

If you want to compile the compilier and interpreter from source, you will need to have the following installed on your system:

1. Make  
2. Clang++-16 (Earlier versions may work but have not been tested)  
3. SDL2 (For the graphics library)  

## **Syntax Highlighting for Visual Studio Code**

https://marketplace.visualstudio.com/items?itemName=MarkCastle.cl  

Extension Name: CastleLang Theme

## **Values**

7 different types of Values

### Number 
    
Holds any type of number, ex: 2, -43, 1.45  
Maps to a double in C++  

### String 
    
Holds any characters that appear between quotation marks  
Maps to a std::string in C++  

### Boolean
     
Holds true or false  
Maps to a bool in C++  

### Null
    
Holds nullptr  
Maps to a nullptr in C++  

### Function
    
Holds a function definition  
Maps to a pointer to a custom function struct in C++, contains the function name, list of parameters, and the bytecode of the function  

### Vector 
    
Holds a list of Values, can be a mix of types  
Maps to a std::vector<Value> in C++  

### Map 
    
Holds a set of keys that map to values  
Maps to a std::map<std::string, Value> in C++  


## **Functions**

All functions must return a value (this extends to stdlib functions). This also means that when calling a function, you must assign the return value to a variable or use it in an expression. 

## **Syntax Examples**

### Assignment/Reassignment
```
let name = "Bob";
let age = 25;

print name + " is " + age + " years old"; // Output: Bob is 25 years old

name = "Alice";
print name; // Output: Alice
```

### Vectors
```
let vec = [1, 2, 3, 4, 5];
print vec[2]; // Output: 3
```

```
let vec = [1, "Bob", null, true,
           [1, 2, 3],
           map{"name" : "Alice"},
           func(){return 5;}
          ];
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

```
let m = map{
    "string" : "Hello",
    "number" : 5,
    "bool" : true,
    "null" : null,
    "vector" : [1, 2, 3],
    "map" : map{"name" : "Alice"},
    "function" : func(){return 5;}
};
```

```
let m = map{
    5 : "Bob",
    true : 25,
    null : "Alice",
    [1, 2, 3] : "Hello",
    map{"name" : "Alice"} : "World",
    func(){return 5;} : "!"
};
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

### Foreach loop
```
let alphabet = map{
    "a": 1,
    "b": 2,
    "c": 3
};
foreach(let key:value in alphabet) {
    print key + " : " + value;
    alphabet[key] = value + 1;
}
// Output:
// a : 1
// b : 2
// c : 3
```

```
let vec = [4, 5, 6];
foreach(let index:element in vec) {
    print index + " : " + element;
    vec[index] = element + 1;
}
// Output:
// 0 : 4
// 1 : 5
// 2 : 6
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

### Command Line Arguments
```
./lii test.cl arg1 arg2 arg3
```

```
print argc; // Output: 3
print argv; // Output: ["arg1", "arg2", "arg3"]
```

## **Standard Library** 

CastleLang has a built in standard library implemented in C++. These functions are able to be called from .cl file by prefixing a function call with '$'. An example of this is -> 
```
let str = "Hello World!";
print $string_length(str);
```

### List of Standard Library Functions

#### String Functions
1. string_length(string) -> number  
    - Returns the length of the string.

...


###  Foreign Function Interface (FFI) 
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
The Tokenizer takes in a file with the .cl file extension and will go through the file line by line creating tokens that represent different parts of the language. Tokens can be keywords, different symbols like semicolons, identifiers, numbers, etc..
    
2. Parsing  
The Parser takes in a list of tokens and creates an abstract syntax tree using recursive decent parsing.

3. Bytecode Generation  
The Bytecode Generator takes in an abstract syntax tree and generates 
    1. bytecode
    2. constants array
        - Numbers, Strings, Functions, null, and bools
    3. variable names array

It puts these three arrays into a .cl_exe file that can be read and then run by the Virtual Machine.

4. Virtual Machine  
The Virtual Machine takes in a cl_exe and steps through the bytecode array. The VM uses a stack to evaluate expressions and call functions.

### Complier/VM Flags

1. -v [-vT -vP -vB -vV]:  
    -v turns on all four flags  
    -vT turns on the verbose Tokenizer flag  
    -vP turns on the verbose Parser flag
    -vB turns on the verbose Bytecode Generator flag
    -vV turns on the verbose Virtual Machine flag

2. -jit [number]:
    -jit flag will run the JIT compiler. The number is the number of times a function must be called before the JIT compiler is run. The JIT compiler will only run if the function is called more than the number specified.

3. -d:
    -d flag will run the VM in debug mode. This allows you to step through the bytecode one instruction at a time and after each instruction the debugger will print useful information.

4. -t:
    -t flag will time each step of the process. This will give you an idea of how long each step takes.


## **JIT Compiler**

The JIT compiler is a separate program that takes in the bytecode and generates a shared object file that can be loaded into the VM. The JIT compiler uses clang++-16 (must have that as a valid command as it is currently hardcoded) to generate the shared object file.

Currently, the JIT compiler is very basic and only generates a shared object file for each function. The JIT compiler does not look ahead in the bytecode to identify chunks of code that would be good to compile together. This is a future plan.

Use the -jit flag and a number to run the JIT compiler. The number is the number of times the function will be called before the JIT compiler is run. The JIT compiler will only run if the function is called more than the number specified.

## **Debugger**

Can run the bytecode in debug mode using the -d flag. This allows you to step through the bytecode one instruction at a time and after each instruction the debugger will print useful information.


## **Known Issues**

Cannot print functions inside of a JITed function.  
Error when running the test:
symbol lookup error: ./jit_functions/jit_0.so: undefined symbol: _Z16display_bytecodeP8function
Test tests_2/types/test_all_types_as_passable.cl Failed  

No checking for the proper number of arguments when calling a function.  

The tokenizer skips invalid characters and does not throw an error.  

## **Future Plans**

1. Expand the standard library  
2. Improve the jit compiler  
    - make the jit compiler look ahead in the bytecode and identify chunks of the code that would be good to compile together. Do this on another thread so that the main thread can continue to run the VM.
