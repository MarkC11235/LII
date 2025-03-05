#"matrices.clh"

// 1 a)

let H = hilbert_matrix(matrix, 11);
let xexact = transpose(create_matrix(matrix, 1, 11, [[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]]));

let b = H * xexact;
// print "b = H * xexact";
// let res = print_matrix(b);

let xapprox = solve_system(H, b);
// print "xapprox = solve_system(H, b)";
// let res = print_matrix(xapprox);


// 1 b)

let e = xapprox - xexact;
// print "e = xapprox - xexact";
// let res = print_matrix(e);
// print "";

let residual = H * xapprox - b;
// print "residual = H * xapprox - b";
// let res = print_matrix(residual);
// print "";

let b_norm = norm(matrix_to_vector(b), "inf");
// print "b_norm = norm(matrix_to_vector(b), 'inf')";
// print b_norm;
// print "";

let xexact_norm = norm(matrix_to_vector(xexact), "inf");
// print "xexact_norm = norm(matrix_to_vector(xexact), 'inf')";
// print xexact_norm;
// print "";

let e_norm = norm(matrix_to_vector(e), "inf");
// print "e_norm = norm(matrix_to_vector(e), 'inf')";
// print e_norm;
// print "";

let resdiual_norm = norm(matrix_to_vector(residual), "inf");
// print "resdiual_norm = norm(matrix_to_vector(residual), 'inf')";
// print resdiual_norm;
// print "";

let relative_error = e_norm / xexact_norm;
// print "relative_error = e_norm / xexact_norm";
// print relative_error;
// print "";

let relative_residual = resdiual_norm / b_norm;
// print "relative_residual = resdiual_norm / b_norm";
// print relative_residual;
// print "";


// 1 c)

let H_norm = matrix_norm(H, "inf");
// print "H_norm = matrix_norm(H, 'inf')";
// print H_norm;
// print "";

let H_inv = inverse(H);
let H_inv_norm = matrix_norm(H_inv, "inf");
// print "H_inv_norm = matrix_norm(H_inv, 'inf')";
// print H_inv_norm;
// print "";

let cond = H_norm * H_inv_norm;
// print "K = H_norm * H_inv_norm";
// print cond;
// print "";

// 1 d)

let check_error_inequality = (1/H_norm * resdiual_norm <= e_norm) 
                             &&
                             (e_norm <= H_inv_norm * resdiual_norm);
print "check_error_inequality: 1/H_norm * resdiual_norm <= e_norm <= H_inv_norm * resdiual_norm";
print check_error_inequality;
print "";

let check_relative_error_inequality = (1/cond * relative_residual <= relative_error) 
                                      &&
                                      (relative_error <= cond * relative_residual);
print "check_relative_error_inequality: 1/cond * relative_residual <= relative_error <= cond * relative_residual";
print check_relative_error_inequality;
print "";
                                