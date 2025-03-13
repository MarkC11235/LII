let pprint = func(f){
    print f;
    return f;
};

let res5 = pprint(func(x){return x;});
// print func(){return 1;};
