// It looks like define is a function call, but it is actually a statement in the language and can be used as a top-level (is alone on a line) statement

// telling compiler we are defining a type, type name, default value of the type, and a function to create an instance of the type
define (type, point, map {"x": 0, "y": 0}, func(x, y) {
    return map {
        "x": x,
        "y": y
    };
});

// define ("type", "int", 0, func(n) {
//     return n;
// });


// telling the compiler we are defining an operation between two types, op symbol, type 1, type 2, and a function to perform the operation
define (op, "+", point, point, func(p1, p2) {
    return point(p1["x"] + p2["x"], p1["y"] + p2["y"]);
});

define (op, "u-", point, func(p) {
    return point(-p["x"], -p["y"]);
});

// // can operate between different types as well
// define ("op", "*", "number", "point", func(n, p) {
//     return point(n * p["x"], n * p["y"]);
// });

// let point p1 = point;
// creating a point using the map constructor
let point up1 = point; // default value
let point up2 = point(1, 2);
// let point p2 = point(3, 4);


// print "p1.x = ", p1["x"], "\n"; // accessing the x property of p1
// print "p1.y = ", p1["y"], "\n"; // accessing the y property of p1

// print "p2.x = ", p2["x"], "\n"; // accessing the x property of p2
// print "p2.y = ", p2["y"], "\n"; // accessing the y property of p2

// // adding two points together using the + operator we defined
// let point p3 = p1 + p2; // this will call the function we defined for the + operator with p1 and p2 as arguments
// print "p3.x = ", p3["x"], "\n"; // accessing the x property of p3
// print "p3.y = ", p3["y"], "\n"; // accessing the y property of p3

// // multiplying a point by a number using the * operator we defined
// let point p4 = 2 * p1; // this will call the function we defined for the * operator with 2 and p1 as arguments
// print "p4.x = ", p4["x"], "\n"; // accessing the x property of p4
// print "p4.y = ", p4["y"], "\n"; // accessing the y property of p4

