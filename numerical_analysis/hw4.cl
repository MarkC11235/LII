#"matrices.clh"

// 1)

//  f = [
//      x_1^3 - 2x_2 - 2   = 0
//      x_1^3 - 5x_3^2 + 7 = 0
//      x_2*x_3^2 - 1      = 0
//      ]

// J = [
//      3x_1^2,  -2    , 0
//      3x_1^2,  0     , -10x_3
//      0     ,  x_3^2 , 2x_2*x_3
//      ]

// let f = func(create_matrix, matrix, x){
//     return create_matrix(matrix, 3, 1, [
//         [x["data"][0][0]^3 - 2*x["data"][1][0] - 2],
//         [x["data"][0][0]^3 - 5*x["data"][2][0]^2 + 7],
//         [x["data"][1][0]*x["data"][2][0]^2 - 1]
//     ]);
// };
// let J = func(create_matrix, matrix, x){
//     return create_matrix(matrix, 3, 3, [
//         [3*x["data"][0][0]^2, -2, 0],
//         [3*x["data"][0][0]^2, 0, -10*x["data"][2][0]],
//         [0, x["data"][2][0]^2, 2*x["data"][1][0]*x["data"][2][0]]
//     ]);
// };

// let x = create_matrix(matrix, 3, 1, [
//     [1],
//     [1],
//     [1]
// ]);

// let x_exact = create_matrix(matrix, 3, 1, [
//     [3^(1/3)],
//     [1/2],
//     [2^(1/2)]
// ]);

// print "Iteration: 0 -------------------------------------------";
// print "x = "; 
// let res = print_matrix(x);

// for(let i = 0; i < 5; i = i + 1){
//     let J_inv = inverse(J(create_matrix, matrix, x));
//     let f_val = f(create_matrix, matrix, x);
//     x = x - J_inv * f_val;

//     print "Iteration: " + (i + 1) + " -------------------------------------------";
//     print "x = "; 
//     let res = print_matrix(x);
//     print "Infinity norm of x - x_exact = " + matrix_norm(x - x_exact, "inf");
// }


// 3)

// a)
let euler_method = func(f, t_0, y_0, t_end, N){
    let h = (t_end - t_0) / N;
    let t = t_0;
    let y = y_0;

    for(let i = 0; i < N; i = i + 1){
        y = y + h * f(t, y);
        t = t + h;
    }

    return y;
};

// b, c)

let f = func(t, y){
    return y^2 + 1 / t^2;
};

let y_exact = func(t){
    return 1 / (2*t) * (3^(1/2) * $tan(3^(1/2)/2 * $ln($abs(t))) - 1);
};

let N_vals = [10, 100, 1000, 10000];

let t_0 = 1;
let y_0 = -1/2;
let t_end = 2;


print "t: [1, 2]";
print "y(1) = -1/2";
print "Exact solution: y(2) = " + y_exact(t_end);
print "";

let y_approx = [];
foreach(let i:n in N_vals){
    let y = euler_method(f, t_0, y_0, t_end, n);
    y_approx = $vector_push(y_approx, y);
    print "N = " + n + ", y(2) = " + y;
    print "";
}

// d)

// // getting the step size
// let x = N_vals;
// foreach(let i:n in x){
//     x[i] = $ln(1 / n);
// }

// let y_error = $vector_length(x) * [0];
// foreach(let i:y in y_approx){
//     y_error[i] = $ln($abs(y - y_exact(t_end)));
// }

// print "logarithm of error vs. logarithm of step size";

// foreach(let i:n in x){
//     print "x = " + x[i] + ", y = " + y_error[i];
// }



