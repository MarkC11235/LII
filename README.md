# **CastleVM** 

### **Intro**

The Castle Vitual Machine runs .cast files. These files are written in the CastleLang programming language. 

To Use:

1. Clone this repository
2. Navigate to the folder
3. Run the command 'make build_bytecode' which will output the castlevm executable file
4. OPTIONAL: add the excutable file to your path to use it from anywhere
5. Run the command 'castlevm file_path.cast'


### **Values**

Types of Values

1. Number (holds any type of number, ex: 2, -43, 1.45)
2. String (holds any characters that appear between quotation marks)
3. Vector (holds a list of Values, can be a mix of types)

### **Syntax Examples**

Assignment and printing
```
let name = "Bob";
let age = 25;

print name + " is " + age + " years old"
```

Functions
```
let factorial = func(n){
    if(n == 0){
        return 1; 
    }
    return n * factorial(n - 1);
};

print factorial(5);

```

Control flow
```
for(let i = 1; i <= 10; i = i + 1){
    if(i == 5){
        print "Halfway";
    }
}
```

Standard Library
```
let str = "Hello World!";
print $string_len(str);
```

### **Standard Library**

CastleLang has a built in standard library implemented in c++. These functions are able to be called from .cast file by prefixing a function call with '$'. An example of this is -> 
```
let str = "Hello World!";
print $string_len(str);
```

The VM has a foreign function interface (FFI) that allows CastleLang to call predefined c++ functions. The interface is a list that contains information about each function,
1. name of function
2. function pointer
3. return type
4. list of parameter types 

The function pointer is created dynamically using templates and lambda functions, so that functions with any return type and parameter types can be used without modifying the interface. 

To add more functions, add another entry to the STD_LIB_FUNCTIONS_DEFINITIONS vector with the proper function information.

### **Process to compile and run CastleLang**

There are four steps to compile and run CastleLang. 
1. Tokenization  
The Tokenizer takes in a file with the .cast file extension and will go through the file line by line creating tokens that represent different parts of the language. Tokens can be keywords, different symbols like semicolons, identifiers, numbers, etc..
    
2. Parsing  
The Parser takes in a list of tokens and creates an abstract syntax tree using recursive decent parsing.

3. Bytecode Generation  
The Bytecode Generator takes in an abstract syntax tree and generates 
    1. bytecode
    2. constants array
    3. variable names array
    4. function definitions

4. Virtual Machine  
The Virtual Machine takes in the bytecode generator output and steps through the bytecode array. The VM uses a stack to evaluate expressions and call functions.


