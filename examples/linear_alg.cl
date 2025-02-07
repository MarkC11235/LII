let A = [
        [1, 5, 0],
        [0, 1, 0],
        [1, 5, 1]
        ];

let b = [17, 3, 21];

let print_matrix = func(A){
    for (let i = 0; i < $vector_length(A); i = i + 1){
        print A[i];
    }
    return 0;
};

let print_augmented_matrix = func(A){
    for (let i = 0; i < $vector_length(A); i = i + 1){
        let row = "";
        for (let j = 0; j < $vector_length(A[i]); j = j + 1){
            if (j == $vector_length(A[i]) - 1){
                let r = A[i][j];
                row = row + "| " + r;
            } else {
                let r = A[i][j] + "";
                let padding = 5 - $string_length(r);
                for (let k = 0; k < padding; k = k + 1){
                    r = r + " ";
                }
                row = row + r + " ";
            }
        }
        print row;
    }
    return 0;
};

let augment = func(A, b, rows, cols){
    // make blank matrix
    let result = [];
    for (let i = 0; i < rows; i = i + 1){
        result = $vector_push(result, []);
        for (let j = 0; j < cols; j = j + 1){
            result[i] = $vector_push(result[i], 0);
        }
    }

    // fill in matrix
    for (let i = 0; i < rows; i = i + 1){
        for (let j = 0; j < cols; j = j + 1){
            if (j < cols - 1){
                result[i][j] = A[i][j];
            } else {
                result[i][j] = b[i];
            }
        }
    }

    return result;
};

let aug = augment(A, b, 3, 4);

// A: matrix, i, j: row indices
let swap_rows = func(A, i, j){
    let temp = A[i];
    A[i] = A[j];
    A[j] = temp;
    return A;
};

// A: matrix, i: row index, c: scalar
let scale_row = func(A, i, c){
    for (let j = 0; j < $vector_length(A[i]); j = j + 1){
        A[i][j] = A[i][j] * c;
    }
    return A;
};

// A: matrix, i, j: row indices, c: scalar
// i is the row being modified, j*c is added to row i
let add_row = func(A, i, j, c){
    for (let k = 0; k < $vector_length(A[i]); k = k + 1){
        A[i][k] = A[i][k] + c * A[j][k];
    }
    return A;
};

let gauss_elimination = func(A){
    let rows = $vector_length(A);
    let cols = $vector_length(A[0]);

    for (let i = 0; i < rows; i = i + 1){
        let pivot = A[i][i];
        // scale row i so that pivot is 1
        if(pivot == 0){
            print "Pivot is 0, cannot continue";
            return A;
        }
        A = scale_row(A, i, 1 / pivot);

        // eliminate all other entries in the pivot column
        for (let j = 0; j < rows; j = j + 1){
            if (j != i){
                A = add_row(A, j, i, -A[j][i]);
            }
        }
    }

    return A;
};

print "Augmented matrix:";
let res = print_augmented_matrix(aug);

print "Gauss elimination:";
let res = print_augmented_matrix(gauss_elimination(aug));


let A = [
        [1, 5, 0],
        [0, 1, 0],
        [1, 5, 1]
        ];
let b = [17, 3, 21];

let x = gauss_elimination(aug);

let print_solution = func(A){
    for (let i = 0; i < $vector_length(A); i = i + 1){
        print "x" + i + " = " + A[i][$vector_length(A[i]) - 1];
    }
    return 0;
};

print "Solution:";
let res = print_solution(x);



