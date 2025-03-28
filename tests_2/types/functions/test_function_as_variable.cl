let func function_reciever = func(function){
    return function();
};

let func one = func(){
    return 1;
};

let func two = func(){
    return 2;
};

print function_reciever(one); // 1
print function_reciever(two); // 2

let func op = func(operation, x, y){
    return operation(x, y);
};

let func add = func(x, y){
    return x + y;
};

let func sub = func(x, y){
    return x - y;
};

print op(add, 1, 1); // 2
print op(sub, 1, 1); // 0