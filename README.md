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

Vectors can store a mix of any type of value.
```
let vec = [1, "Bob", null, true,
           [1, 2, 3],
           map{"name" : "Alice"},
           func(){return 5;}
          ];
```


### Maps
Map keys must be unique strings.
```
let Person = map{
    "name" : "Bob",
    "age" : 25
};

print Person["name"]; // Output: Bob
print Person["age"]; // Output: 25
```

Maps can store a mix of any type of value.
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

Can use any type of value as a key, the value will be coerced to a string.
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
Functions must contain a return statement, so this means that you must use that return value in an expression or assign it to a variable.  
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
#### for loop
For loops expect an assignment, a condition(expr), and a variable update, in that order.
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

### foreach loop
Can iterate over a map or vector. The key and value are assigned to the variables in the foreach loop. The key is a string for maps and a number for vectors. The value is the value at the key in the map or vector.
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

#### break and continue
Break will exit the loop and go to the next line of code.
Continue will skip the rest of the loop iteration and go to the next iteration.
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
#### if, else if, and else
```
let x = 5;
if(x == 5){
    print "x is 5";
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

```
let x = 6;
if(x == 5){
    print "x is 5";
}
else if(x == 6){
    print "x is 6";
}
else{
    print "x is not 5 or 6";
}
// Output: x is 6
```


### Standard Library Call
Standard library functions can be called by prefixing the function call with '$'.
```
let str = "Hello World!";
print $string_len(str); // Output: 12
```

### Command Line Arguments
Command line arguments can be accessed in the .cl file by using the argc and argv variables. These variables are automatically created when the .cl file is run from the command line.
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

List of all standard library functions at the bottom of this document.

###  Foreign Function Interface (FFI) 
The VM has a foreign function interface (FFI) that allows CastleLang to call predefined C++ functions. The interface is a list that contains information about each function,
1. name of function
2. function pointer
3. return type
4. list of parameter types 

The function pointer is created dynamically using templates and lambda functions, so that functions with any return type and parameter types can be used without modifying the interface. 

To add more functions, add another entry to the STD_LIB_FUNCTIONS_DEFINITIONS vector with the proper function information.

## Operator Overloading
Operator overloading is a way to define how maps with __type field should behave when an operator is used on them.
For operators with two operands, both operands must have the same __type field. The define statement globally defines the operator for that type. Can overload any operator except for function calls '()' and access '[]'.


The following is an example of how to use operator overloading for a matrix type:  
The negative sign must be defined as "u-" because the "-" operator is already used for subtraction.  

```
let matrix = map {
    "__type": "matrix",
    "rows": 0,
    "cols": 0,
    "data": []
};

define "+" in "matrix" as func(a, b){
    let a_rows = a["rows"];
    let a_cols = a["cols"];
    let b_rows = b["rows"];
    let b_cols = b["cols"];
    if (a_rows != b_rows || a_cols != b_cols) {
        print "Error: incompatible matrix sizes for addition";
        return null; // BECAUSE NULL CAN'T BE OPERATED ON
    }
    for (let i = 0; i < a_rows; i = i + 1) {
        for (let j = 0; j < a_cols; j = j + 1) {
            a["data"][i][j] = a["data"][i][j] + b["data"][i][j];
        }
    }
    return a;
};

define "-" in "matrix" as func(a, b){
    let a_rows = a["rows"];
    let a_cols = a["cols"];
    let b_rows = b["rows"];
    let b_cols = b["cols"];
    if (a_rows != b_rows || a_cols != b_cols) {
        print "Error: incompatible matrix sizes for subtraction";
        return null; // BECAUSE NULL CAN'T BE OPERATED ON
    }
    for (let i = 0; i < a_rows; i = i + 1) {
        for (let j = 0; j < a_cols; j = j + 1) {
            a["data"][i][j] = a["data"][i][j] - b["data"][i][j];
        }
    }
    return a;
};

define "*" in "matrix" as func(a, b){
    a["data"] = $matrix_multiply(a["data"], b["data"]);
    return a;
};

define "u-" in "matrix" as func(a){
    for (let i = 0; i < a["rows"]; i = i + 1) {
        for (let j = 0; j < a["cols"]; j = j + 1) {
            a["data"][i][j] = -a["data"][i][j];
        }
    }
    return a;
};

let m1 = matrix;
m1["rows"] = 2;
m1["cols"] = 2;
m1["data"] = [[1, 2],
               [3, 4]];

let m2 = matrix;
m2["rows"] = 2;
m2["cols"] = 2;
m2["data"] = [[5, 6],
               [7, 8]];


let m3 = m1 + m2;
let m4 = m1 - m2;
let m5 = m1 * m2;
let m6 = -m1;

print "m1 + m2";
print m3; // [[6, 8], [10, 12]]
print "m1 - m2";
print m4; // [[-4, -4], [-4, -4]]
print "m1 * m2";
print m5; // [[19, 22], [43, 50]]
print "-m1";
print m6; // [[-1, -2], [-3, -4]]
```




## **Process to compile and run CastleLang**

There are four steps to compile and run CastleLang. 
1. Tokenization  
The Tokenizer takes in a file with the .cl file extension and will go through the file line by line creating tokens that represent different parts of the language. Tokens can be keywords, different symbols like semicolons, identifiers, numbers, etc.. In this step, include files are also processed and the tokens are added to the list of tokens, where the include statement was. Also, escape characters that appear in strings are processed in this step.
    
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



## Standard Library Functions

The `STD_LIB_FUNCTIONS_DEFINITIONS` vector contains the definitions of all standard library functions available in CastleLang. Each function is defined with its name, a function pointer, its return type, and its argument types.

### Test Functions
- **do_nothing**: `int do_nothing()`
  - Does nothing and returns 0.
- **test**: `double test()`
  - Returns the value 42.0.
- **inc**: `double inc(double a)`
  - Increments the given number by 1.

### String Functions
- **string_join**: `std::string string_join(std::string, std::string)`
  - Joins two strings together.
- **string_substring**: `std::string string_substring(std::string, int, int)`
  - Returns a substring from the given string starting at the specified position and of the specified length.
- **string_length**: `int string_length(std::string)`
  - Returns the length of the given string.
- **char_at**: `std::string char_at(std::string, int)`
  - Returns the character at the specified position in the given string.
- **replace_char**: `std::string replace_char(std::string, int, std::string)`
  - Replaces the character at the specified position in the given string with another character.
- **print_colored_text**: `int print_colored_text(std::string, std::string)`
  - Prints the specified text in the specified color.
- **string_to_vector**: `std::vector<Value> string_to_vector(std::string)`
  - Converts the given string to a vector of characters.
- **string_split**: `std::vector<Value> string_split(std::string, std::string)`
  - Splits the given string by the specified delimiter and returns a vector of substrings.

### Vector Functions
- **vector_create**: `std::vector<Value> vector_create(int, Value)`
  - Creates a vector of the specified size, filled with the given value.
- **vector_length**: `int vector_length(std::vector<Value>)`
  - Returns the length of the given vector.
- **vector_push**: `std::vector<Value> vector_push(std::vector<Value>, Value)`
  - Pushes a value onto the end of the given vector.
- **vector_pop**: `std::vector<Value> vector_pop(std::vector<Value>)`
  - Pops a value from the end of the given vector.
- **vector_insert**: `std::vector<Value> vector_insert(std::vector<Value>, int, Value)`
  - Inserts a value at the specified position in the given vector.
- **vector_remove**: `std::vector<Value> vector_remove(std::vector<Value>, int)`
  - Removes the value at the specified position in the given vector.
- **vector_clear**: `std::vector<Value> vector_clear(std::vector<Value>)`
  - Clears all values from the given vector.
- **vector_get**: `Value vector_get(std::vector<Value>, int)`
  - Gets the value at the specified position in the given vector.
- **vector_set**: `std::vector<Value> vector_set(std::vector<Value>, int, Value)`
  - Sets the value at the specified position in the given vector.
- **vector_slice**: `std::vector<Value> vector_slice(std::vector<Value>, int, int)`
  - Returns a slice of the given vector from the specified start position to the specified end position.
- **vector_reverse**: `std::vector<Value> vector_reverse(std::vector<Value>)`
  - Reverses the given vector.
- **vector_join**: `std::vector<Value> vector_join(std::vector<Value>, std::vector<Value>)`
  - Joins two vectors together.

### Map Functions
- **map_size**: `int map_size(std::map<std::string, Value>)`
  - Returns the size of the given map.
- **map_join**: `std::map<std::string, Value> map_join(std::map<std::string, Value>, std::map<std::string, Value>)`
  - Joins two maps together.

### File Functions
- **file_write**: `int file_write(std::string, std::string)`
  - Writes the given string to the specified file.
- **file_write_lines**: `int file_write_lines(std::string, std::vector<Value>)`
  - Writes the given vector of strings to the specified file, each string on a new line.
- **file_read**: `std::string file_read(std::string)`
  - Reads the contents of the specified file and returns it as a string.
- **file_read_lines**: `std::vector<Value> file_read_lines(std::string)`
  - Reads the contents of the specified file and returns it as a vector of strings, each line as a separate string.
- **stdin_read**: `std::string stdin_read()`
  - Reads input from standard input and returns it as a string.
  - Waits for the user to press enter.
- **csv_write**: `int csv_write(std::string, std::vector<Value>)`
  - Writes the given vector of vectors of strings to the specified CSV file.
- **csv_read**: `std::vector<Value> csv_read(std::string)`
  - Reads the contents of the specified CSV file and returns it as a vector of vectors of strings.
- **run_python_file**: `int run_python_file(std::string)`
  - Runs the specified Python file.
- **map_to_json**: `std::string map_to_json(std::map<std::string, Value>)`
  - Converts the given map to a JSON string.
- **json_to_map**: `std::map<std::string, Value> json_to_map(std::string)`
  - Converts the given JSON string to a map.

### Graphics Functions
- **get_events**: `std::vector<Value> get_events()`
  - Gets the current events from the graphics system.
- **init_graphics**: `int init_graphics(std::string, int, int)`
  - Initializes the graphics system with the specified title, width, and height.
- **close_graphics**: `int close_graphics()`
  - Closes the graphics system.
- **clear_screen**: `int clear_screen()`
  - Clears the screen.
- **update_screen**: `int update_screen()`
  - Updates the screen.
- **draw_rect**: `int draw_rect(int, int, int, int)`
  - Draws a rectangle with the specified position and size.

### Random Functions
- **random_int**: `int random_int(int, int)`
  - Returns a random integer between the specified minimum and maximum values.

### Misc Functions
- **exit_program**: `int exit_program(std::string)`
  - Exits the program with the specified message.
- **wait**: `int wait(double)`
  - Waits for the specified number of seconds.
- **system_command**: `int system_command(std::string)`
  - Executes the specified system command.

### HTTP Functions
- **start_server**: `int start_server(int)`
  - Starts an HTTP server on the specified port.
- **stop_server**: `int stop_server()`
  - Stops the HTTP server.
- **pop_request**: `std::map<std::string, Value> pop_request()`
  - Pops the next request from the HTTP server.
- **push_response**: `int push_response(int, std::map<std::string, Value>)`
  - Pushes a response to the HTTP server.
- **server_should_close**: `bool server_should_close()`
  - Checks if the HTTP server should close.

### HTTP Client Functions
- **send_request**: `std::map<std::string, Value> send_request(std::string, int, std::map<std::string, Value>)`
  - Sends an HTTP request and returns the response.

### Matrix Operations
- **matrix_multiply**: `std::vector<Value> matrix_multiply(std::vector<Value>, std::vector<Value>)`
  - Multiplies two matrices together.



## **Version History**

### Version 1.0.0
- Hash: 422453b6faf2f7a5409904e811ec3271e4806603
- Date: 3/13/2025
- Compiler infrastructure rework complete