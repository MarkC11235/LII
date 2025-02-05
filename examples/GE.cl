#"./linear_algebra.clh"

let A = [
    [3, 2, -4],
    [-4, 5, -1],
    [2, -3, 5]
];

let b = [-5, 11, 3];

let augmented = augment_matrix(A, b);
let res = pretty_print_augmented(augmented);

let REF_augmented = REF_GE(augmented);
let res = pretty_print_augmented(REF_augmented);
let x = Back_Substitution(REF_augmented);
print "Solution: " + x;