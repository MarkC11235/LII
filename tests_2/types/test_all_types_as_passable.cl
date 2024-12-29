let pprint = func(x){
    print x;
    return x;
};

let res = pprint(1);
let res2 = pprint("hello");
let res3 = pprint([1, 2, 3]);
let res4 = pprint(map{let vec1 = [1, 2, 3]; let vec2 = [4, 5, 6];});
let res5 = pprint(func(){return 1;});
let res6 = pprint(true);
let res7 = pprint(null);

