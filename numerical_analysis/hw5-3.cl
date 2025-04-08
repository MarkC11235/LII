let RK4 = func(f, y0, t0, t1, N){
    let h = (t1 - t0) / N;
    let t = t0;
    let y = y0;
    
    for (let i = 0; i < N; i = i + 1){
        let f_val = f(t, y);
        let k1 = h * f_val;
        let k2 = h * f(t + h / 2, y + k1 / 2);
        let k3 = h * f(t + h / 2, y + k2 / 2);
        let k4 = h * f(t + h, y + k3);
        y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        t = t + h;
    }
    
    return y;
};

let f = func(t, y) { return y^2 + 1/t^2; };

let y0 = -0.5;
let t0 = 1;
let t1 = 2;

// b)
let res10 = RK4(f, y0, t0, t1, 10);
let res100 = RK4(f, y0, t0, t1, 100);
let res1000 = RK4(f, y0, t0, t1, 1000);
let res10000 = RK4(f, y0, t0, t1, 10000);

print "Results for RK4 Method with different N values:";
print "N = 10, y(2) = " + res10;
print "N = 100, y(2) = " + res100;
print "N = 1000, y(2) = " + res1000;
print "N = 10000, y(2) = " + res10000;

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