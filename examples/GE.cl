#"./linear_algebra.clh"

// let A = [
//     [3, 2, -4],
//     [-4, 5, -1],
//     [2, -3, 5]
// ];

// let b = [-5, 3, 11];


// let augmented = augment_matrix(A, b);
// let res = pretty_print_augmented(augmented);

// let REF_augmented = REF_GE(augmented);
// let res = pretty_print_augmented(REF_augmented);
// let x = Back_Substitution(REF_augmented);
// print "Solution: " + x;

// let A = [
//     [3, 2, -4],
//     [2, -3, 5],
//     [-4, 5, -1]
// ];

// let b = [-5, 11, 3];

// let augmented = augment_matrix(A, b);
// let res = pretty_print_augmented(augmented);

// let REF_augmented = REF_GE(augmented);
// let res = pretty_print_augmented(REF_augmented);
// let x = Back_Substitution(REF_augmented);
// print "Solution: " + x;

let A = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
];

let b = [-5, 3, 11];

print "det(A) = " + determinant(A);
// let augmented = augment_matrix(A, b);
// let res = pretty_print_augmented(augmented);

// let REF_augmented = REF_GE(augmented);
// let res = pretty_print_augmented(REF_augmented);
// let x = Back_Substitution(REF_augmented);
// print "Solution: " + x;
