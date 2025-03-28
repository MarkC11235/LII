let func a = func(){
    let func p = func(){
        return 1;
    };
    let func q = func(){
        return 2;
    };

    print p();
    print q();

    return 3;
};

print a();