let pprint = func(f){
    // let f = func(){
    //     print 1;
    //     return 1;
    // };
    print f(1, 2);
    return f;
};

// let res = pprint(func(){
//     return 1;
// });

let add = func(x, y){
    return x + y;
};

let res = pprint(add);

// let res = pprint(map{
//     let x = 1;
//     let y = 2;
// });

