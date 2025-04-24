let f = func(y, t){ return 1 + y/t; };
let fp = func(y, t){ return 1/t; };
let y0 = 1;
let t0 = 1;
let t1 = 6;

let N = [10, 100, 1000];

let AB2 = func(f, fp, y0, t0, t1, N){
    let h = (t1 - t0) / N;
    let t = [t0, t0 + h];
    // second order Taylor method
    let y = [y0, y0 + h * f(y0, t0) + h * h / 2 * fp(y0, t0)]; 
    let f_val = [f(y0, t0), f(y[1], t[1])];

    // Adams-Bashforth 2nd order method
    for (let i = 2; i < N; i = i + 1){
        t = $vector_push(t, t[i - 1] + h);  
        y = $vector_push(y, y[i - 1] + h / 2 * (3 * f_val[i - 1] - f_val[i - 2]));
        f_val = $vector_push(f_val, f(y[i], t[i]));
    }
    
    // last step, doesn't get computed in the loop because we need the last two f_val
    return y[N - 1] + h / 2 * (3 * f_val[N - 1] - f_val[N - 2]); 
};

let res10 = AB2(f, fp, y0, t0, t1, N[0]);
let res100 = AB2(f, fp, y0, t0, t1, N[1]);
let res1000 = AB2(f, fp, y0, t0, t1, N[2]);
print "Results for Adams-Bashforth 2nd Order Method with different N values:";
print "N = 10, y(6) = " + res10;
print "N = 100, y(6) = " + res100;
print "N = 1000, y(6) = " + res1000;

let y_exact = func(t){ return t * (1 + $ln(t)); };
print "Exact solution y(6) = " + y_exact(6);

let ln_errors = [
    $ln($abs(y_exact(t1) - res10)),
    $ln($abs(y_exact(t1) - res100)),
    $ln($abs(y_exact(t1) - res1000))
];

let ln_h = [
    $ln(1 / N[0]),
    $ln(1 / N[1]),
    $ln(1 / N[2])
];

print "points for ln(h) and ln(error):";
print "ln(h), ln(error)";
for (let i = 0; i < 3; i = i + 1){
    print ln_h[i] + ", " + ln_errors[i];
}