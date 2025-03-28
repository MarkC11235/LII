let func a = func(){
    let func x = func(){
        return 1;
    };

    return x();
};

let func b = func(){
    let func x = func(){
        return 2;
    };

    return x();
};

print a();
print b();