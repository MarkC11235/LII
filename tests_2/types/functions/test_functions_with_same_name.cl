let a = func(){
    let x = func(){
        return 1;
    };

    return x();
};

let b = func(){
    let x = func(){
        return 2;
    };

    return x();
};

print a();
print b();