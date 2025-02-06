// print func(a, b){return a + b;}(1, 2);

// print 1 + 2 * 7 / 3[1] || true + 7 && false;


// print [1,2,3](1, 2 + 3) + 6;

// print 6 ^ 2 * 2 + 5 * 6; // 102

// print 1 - 2; // -1

// print 1 + (2*4); // 9

// print map{"name": "mark"}; // {"name":"mark"}

// print 4 / 5; // 0.8

// print 4 % 5; // 4

// print 4 < 5; // true

// print 4 > 5; // false

// print [1, 2, 3][1]; // 2

// print func(a, b){
//     print a + b;
//     return func(c, d){
//         return [c, d];
//     };
// }(1, 2)(3, 4)[0]; // 3
//                   // 3

// print 1+-1--1;

// let add = func(a, b){
//     return a + b;
// };

// let op = func(op, a, b){
//     return op(a, b);
// };

// print op(add, 1, 2); // 3

// let factorial = func(factorial, n){
//     if (n <= 1) {
//         return 1;
//     } else {
//         return n * factorial(factorial, n - 1);
//     }
// };

// print factorial(factorial, 5); // 120


for(let cond = 0; cond < 5; cond = cond + 1){
    if(cond == 1){
        print "cond is 1";
    }
    else if(cond == 2){
        print "cond is 2";
    }
    else if(cond == 3){
        print "cond is 3";
    }
    else{
        print "cond is not 1, 2 or 3";
    }
}

print "done";
