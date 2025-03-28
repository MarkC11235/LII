let func pprint = func(x){
    print x;
    return 0;
};

let number res = pprint(1);
let number res2 = pprint("hello");
let number res3 = pprint([1, 2, 3]);
let number res4 = pprint(map{"vec1" : [1, 2, 3], "vec2" : [4, 5, 6]});
let number res5 = pprint(func(){return 1;});
let number res6 = pprint(true);
let number res7 = pprint(null);

