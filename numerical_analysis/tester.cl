#"./matrices.clh"

let B = create_matrix(3, 3, [
    [1, 2, 1],
    [3, -1, 0],
    [2, 3, 2]
]);

let Bp = create_matrix(3, 3, [
    [-1, 3, 1],
    [0, 1, 0],
    [2, -1, 1]
]);

let A = B * inverse(Bp);
let res = print_matrix(A);