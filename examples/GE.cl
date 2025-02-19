#"./linear_algebra.clh"

// // 1 

// let A = [
//     [3, 1, 4, -1],
//     [2, -2, -1, 2],
//     [5, 7, 14, -8],
//     [1, 3, 2, 4]
// ];

// let b = [7, 1, 20, -4];

// let augmented = augment_matrix(A, b);
// print "Augmented Matrix:";
// let res = pretty_print_augmented(augmented);

// // 1 a)

// // without partial pivoting
// print "REF without partial pivoting:";
// let REF_augmented = REF_GE(augmented);
// if(REF_augmented != null){
//     let res = pretty_print_augmented(REF_augmented);
//     let x = Back_Substitution(REF_augmented);
//     print "Solution: " + x;
// }
// print "";

// // with partial pivoting
// print "REF with partial pivoting:";
// let REF_augmented = REF_GE_partial_pivoting(augmented);
// if(REF_augmented != null){
//     let res = pretty_print_augmented(REF_augmented);
//     let x = Back_Substitution(REF_augmented);
//     print "Solution: " + x;
// }

// // 1 b)

// // with partial pivoting and row interchange vector
// print "REF with partial pivoting and row interchange vector:";
// let res = REF_GE_partial_pivoting_row_interchanging_vector(augmented);
// let REF_augmented = res[0];
// let row_interchange_vector = res[1];
// if(REF_augmented != null){
//     let res = pretty_print_augmented(REF_augmented);
//     let x = Back_Substitution_row_interchanging_vector(REF_augmented, row_interchange_vector);
//     print "Solution: " + x;
// }


// 2

let A = [
    [1, 2, 3],
    [2, 4-$pow(10, -12), 7],
    [1/5, -3/7, 2/5]
];

let xexact = [1, 2, 3];

let b = matrix_vector_multiplication(A, xexact);

let augmented = augment_matrix(A, b);
print "Augmented Matrix:";
let res = pretty_print_augmented(augmented);
let REF_augmented = REF_GE(augmented);
let x = null;
if(REF_augmented != null){
    print "REF:";
    let res = pretty_print_augmented(REF_augmented);
    x = Back_Substitution(REF_augmented);
    print "Solution: " + x;
}

let diff = [];
for(let i = 0; i < $vector_length(x); i = i + 1){
    diff = diff + [x[i] - xexact[i]];
}
print "One Norm: " + norm(diff, "1");
print "Two Norm: " + norm(diff, "2");
print "Infinity Norm: " + norm(diff, "inf");

