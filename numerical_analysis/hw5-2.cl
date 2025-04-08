let Taylor_Method_Order_3 = func(f, fp, fpp, y0, t0, t1, N){
    let h = (t1 - t0) / N;
    let t = t0;
    let y = y0;
    
    for (let i = 0; i < N; i = i + 1){
        let f_val = f(t, y);
        let fp_val = fp(t, y);
        let fpp_val = fpp(t, y);
        y = y + h * f_val + (h^2 / 2) * fp_val + (h^3 / 6) * fpp_val;
        t = t + h;
    }
    
    return y;
};

let f = func(t, y) { return y^2 + 1/t^2; };
let fp = func(t, y) { return -2/t^3 + 2*y^3 + 2*y/t^2; };
let fpp = func(t, y) { return 6*y^4 + 8*y^2/t^2 - 4*y/t^3 + 8/t^4; };
let y0 = -0.5;
let t0 = 1;
let t1 = 2;


// b)
let res10 = Taylor_Method_Order_3(f, fp, fpp, y0, t0, t1, 10);
let res100 = Taylor_Method_Order_3(f, fp, fpp, y0, t0, t1, 100);
let res1000 = Taylor_Method_Order_3(f, fp, fpp, y0, t0, t1, 1000);
let res10000 = Taylor_Method_Order_3(f, fp, fpp, y0, t0, t1, 10000);

print "Results for Taylor Method of Order 3 with different N values:";
print "N = 10, y(2) = " + res10;
print "N = 100, y(2) = " + res100;
print "N = 1000, y(2) = " + res1000;
print "N = 10000, y(2) = " + res10000;


// c)
let y_exact = func(t){ return 1 / (2*t) * (3^(1/2) * $tan(3^(1/2)/2 * $ln($abs(t))) - 1); };
print "Exact solution y(2) = " + y_exact(2);

let ln_errors = [
    $ln($abs(y_exact(t1) - res10)),
    $ln($abs(y_exact(t1) - res100)),
    $ln($abs(y_exact(t1) - res1000)),
    $ln($abs(y_exact(t1) - res10000))
];

let ln_h = [
    $ln(1 / 10),
    $ln(1 / 100),
    $ln(1 / 1000),
    $ln(1 / 10000)
];

print "points for ln(h) and ln(error):";
print "ln(h), ln(error)";
for (let i = 0; i < 4; i = i + 1){
    print ln_h[i] + ", " + ln_errors[i];
}