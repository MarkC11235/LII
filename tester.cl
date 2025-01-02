let x = func(){
    return 1;
};

let main = func(){
    let x = func(){
        return 2;
    };
    return x();
};

print main();   