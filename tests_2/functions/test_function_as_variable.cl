let function_reciever = func(function){
    return function();
};

let one = func(){
    return 1;
};

let two = func(){
    return 2;
};

print function_reciever(one); // 1
print function_reciever(two); // 2

let op = func(operation, x, y){
    return operation(x, y);
};

let add = func(x, y){
    return x + y;
};

let sub = func(x, y){
    return x - y;
};

print op(add, 1, 1); // 2
print op(sub, 1, 1); // 0