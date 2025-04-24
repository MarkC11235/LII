global PI = 3.14159;
global e = 2.71828;

let f = func(t, y){ return -y + $sin(t); };
let y0 = 1;
let t0 = PI;
let t1 = 2*PI;

let N = [5, 10, 100, 1000];

global RK4 = func(f, t, y, h){
    let k1 = h * f(t, y);
    let k2 = h * f(t + h / 2, y + k1 / 2);
    let k3 = h * f(t + h / 2, y + k2 / 2);
    let k4 = h * f(t + h, y + k3);
    y = y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    
    return y;
};

let AM3 = func(f, y0, t0, t1, N){
    let h = (t1 - t0) / N;
    let t = [0] * (N + 1);
    let y = [0] * (N + 1);
    t[0] = t0;
    y[0] = y0;

    // get the first four points using RK4
    for(let i = 0; i < 3; i = i + 1){
        t[i + 1] = t[i] + h;
        y[i + 1] = RK4(f, t[i], y[i], h);
    }

    // Adams-Moulton 3-step method using AB4 predictor
    let f_val = [0] * (N + 1);
    f_val[0] = f(t[0], y[0]);
    f_val[1] = f(t[1], y[1]);
    f_val[2] = f(t[2], y[2]);
    f_val[3] = f(t[3], y[3]);

    for (let i = 4; i <= N; i = i + 1){
        t[i] = t[i - 1] + h;
        let y_pred = y[i - 1] + h / 24 * (55 * f_val[i - 1] - 59 * f_val[i - 2] + 37 * f_val[i - 3] - 9 * f_val[i - 4]); // get the predicted y with AB4
        let f_val_pred = f(t[i], y_pred); // get the next f_val with the predicted y

        // now correct the predicted y with the Adams-Moulton method
        y[i] = y[i - 1] + h / 24 * (9 * f_val_pred + 19 * f_val[i - 1] - 5 * f_val[i - 2] + f_val[i - 3]);
        f_val[i] = f(t[i], y[i]); // get the next f_val with the corrected y
    }

    // return the last y value
    return y[N];
};


let res5 = AM3(f, y0, t0, t1, N[0]);
let res10 = AM3(f, y0, t0, t1, N[0]);
let res100 = AM3(f, y0, t0, t1, N[1]);
let res1000 = AM3(f, y0, t0, t1, N[2]);
print "Results for AM 3-step Method with different N values:";
print "N = 5, y(2PI) = " + res5;
print "N = 10, y(2PI) = " + res10;
print "N = 100, y(2PI) = " + res100;
print "N = 1000, y(2PI) = " + res1000;

let y_exact = func(t){ return (1/2) * (e^(PI - t) + $sin(t) - $cos(t)); };
print "Exact solution y(2PI) = " + y_exact(2*PI);

let ln_errors = [
    $ln($abs(y_exact(t1) - res5)),
    $ln($abs(y_exact(t1) - res10)),
    $ln($abs(y_exact(t1) - res100)),
    $ln($abs(y_exact(t1) - res1000))
];

let ln_h = [
    $ln(1 / N[0]),
    $ln(1 / N[1]),
    $ln(1 / N[2]),
    $ln(1 / N[3])
];

print "points for ln(h) and ln(error):";
print "ln(h), ln(error)";
for (let i = 0; i < 4; i = i + 1){
    print ln_h[i] + ", " + ln_errors[i];
}