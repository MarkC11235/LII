// 2)
// let newtons_method = func(f, df, x0, tol, max_iter){
//     print "Newton's method: starting at x0 = " + x0;
//     let abs = func(x){if (x < 0){return -x;} return x;};
//     let x = x0;
//     for (let iter = 0; iter < max_iter; iter = iter + 1){
//         let x_next = x - f(x) / df(x);
//         print "Iteration " + iter + ": x = " + x_next;
//         if (abs(x_next - x) < tol){
//             return x_next;
//         }
//         x = x_next;
//     }
//     print "Error: Newton's method did not converge";
//     return null;
// };

// let f = func(x){return x^3 + 3*x^2 - 4*x - 12;};
// let df = func(x){return 3*x^2 + 6*x - 4;};

// let tol = 10^(-14);
// let max_iter = 1000;

// let x0 = -0.1;
// let root = newtons_method(f, df, x0, tol, max_iter);
// print "Root of f(x) = x^3 + 3x^2 - 4x - 12 is: " + root + "\n";

// x0 = -2.45;
// root = newtons_method(f, df, x0, tol, max_iter);
// print "Root of f(x) = x^3 + 3x^2 - 4x - 12 is: " + root + "\n";

// x0 = 5;
// root = newtons_method(f, df, x0, tol, max_iter);
// print "Root of f(x) = x^3 + 3x^2 - 4x - 12 is: " + root + "\n";



// 3)

// a)
let bisection_method = func(f, a, b, tol, max_iter){
    print "Bisection method: starting with a = " + a + " and b = " + b;
    let abs = func(x){if (x < 0){return -x;} return x;};
    if (f(a) * f(b) > 0){
        print "Error: f(a) and f(b) must have opposite signs";
        return null;
    }
    for (let iter = 0; iter < max_iter; iter = iter + 1){
        let c = (a + b) / 2;
        print "Iteration " + iter + ": a = " + a + ", b = " + b + ", abs(a-b) = " + abs(a-b);
        if (abs(b - a) < tol){
            return c;
        }
        if (f(c) == 0){
            return c;
        }
        if (f(a) * f(c) < 0){
            b = c;
        }
        else{
            a = c;
        }
    }
    print "Error: Bisection method did not converge";
    return null;
};


// b)
let secant_method = func(f, x0, x1, tol, max_iter){
    print "Secant method: starting with x0 = " + x0 + " and x1 = " + x1;
    let abs = func(x){if (x < 0){return -x;} return x;};
    for (let iter = 0; iter < max_iter; iter = iter + 1){
        let x_next = x1 - f(x1) * (x1 - x0) / (f(x1) - f(x0));
        print "Iteration " + iter + ": x_n = " + x_next + ", abs(x_n - x_n-1) = " + abs(x_next - x1);
        if (abs(x_next - x1) < tol){
            return x_next;
        }
        x0 = x1;
        x1 = x_next;
    }
    print "Error: Secant method did not converge";
    return null;
};


// c)

let f = func(x){return x^3 + 3*x^2 - 4*x - 12;};
let tol = 10^(-14);
let max_iter = 1000;

let a = -4;
let b = -2.5;

let root = bisection_method(f, a, b, tol, max_iter);
print "Root of f(x) = x^3 + 3x^2 - 4x - 12 using bisection method is: " + root + "\n";

let root = secant_method(f, a, b, tol, max_iter);
print "Root of f(x) = x^3 + 3x^2 - 4x - 12 using secant method is: " + root + "\n";