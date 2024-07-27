let op = func(operation, x, y){
    return operation(x, y);
};

let add = func(x, y){
    return x + y;
};

let sub = func(x, y){
    return x - y;
};

print op(add, 1, 2); // 3
print op(sub, 1, 2); // -1
