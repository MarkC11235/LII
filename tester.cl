// It looks like define is a function call, but it is actually a statement in the language and can be used as a top-level statement

// telling compiler we are defining a type, type name, default value of the type, and a function to create an instance of the type
define ("type", "point", map {"x": 0, "y": 0}, func(x, y) {
    return {
        "x": x,
        "y": y
    };
});

// define ("type", "int", 0, func(n) {
//     return n;
// });


// telling the compiler we are defining an operation between two types, op symbol, type 1, type 2, and a function to perform the operation
define ("op", "+", "point", "point", func(p1, p2) {
    return point(p1.x + p2.x, p1.y + p2.y);
});

// can operate between different types as well
define ("op", "*", "number", "point", func(n, p) {
    return point(n * p.x, n * p.y);
});