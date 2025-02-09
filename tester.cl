// let new_point = func(x, y, z) {
//     return map{
//         "__type": "3point",
//         "__add" : func(a, b) {
//             a["x"] = a["x"] + b["x"];
//             a["y"] = a["y"] + b["y"];
//             a["z"] = a["z"] + b["z"];
//             return a;
//         },
//         "__sub" : func(a, b) {
//             a["x"] = a["x"] - b["x"];
//             a["y"] = a["y"] - b["y"];
//             a["z"] = a["z"] - b["z"];
//             return a;
//         },
//         "x": x,
//         "y": y,
//         "z": z
//     };
// };

// let print_point = func(p) {
//     print "x: " + p["x"] + ", y: " + p["y"] + ", z: " + p["z"];
//     return 0;
// };

// let p1 = new_point(1, 2, 3);
// let p2 = new_point(4, 5, 6);
// let p3 = p1 + p2;

// let res = print_point(p1);
// res = print_point(p2);
// res = print_point(p3);

// let p4 = p1 - p2;
// res = print_point(p4);





let new_matrix = func (rows, cols, data) {
    return map {
        "__type": "matrix",
        "__+" : func(a, b) {
            let a_rows = a["rows"];
            let a_cols = a["cols"];
            let b_rows = b["rows"];
            let b_cols = b["cols"];
            if (a_rows != b_rows || a_cols != b_cols) {
                print "Error: incompatible matrix sizes for addition";
                return 0;
            }
            for (let i = 0; i < a_rows; i = i + 1) {
                for (let j = 0; j < a_cols; j = j + 1) {
                    a["data"][i][j] = a["data"][i][j] + b["data"][i][j];
                }
            }
            return a;
        },
        "__-" : func(a, b) {
            let a_rows = a["rows"];
            let a_cols = a["cols"];
            let b_rows = b["rows"];
            let b_cols = b["cols"];
            if (a_rows != b_rows || a_cols != b_cols) {
                print "Error: incompatible matrix sizes for subtraction";
                return 0;
            }
            for (let i = 0; i < a_rows; i = i + 1) {
                for (let j = 0; j < a_cols; j = j + 1) {
                    a["data"][i][j] = a["data"][i][j] - b["data"][i][j];
                }
            }
            return a;
        },
        "__*" : func(a, b) {
            // let a_rows = a["rows"];
            // let a_cols = a["cols"];
            // let b_rows = b["rows"];
            // let b_cols = b["cols"];
            // if (a_cols != b_rows) {
            //     print "Error: incompatible matrix sizes for multiplication";
            //     return 0;
            // }
            // let res = a; // duplicate a (kinda makes every map a constructor for itself as long as you clear the data)
            // for (let i = 0; i < a_rows; i = i + 1) {
            //     for (let j = 0; j < b_cols; j = j + 1) {
            //         res["data"][i][j] = 0;
            //         for (let k = 0; k < a_cols; k = k + 1) {
            //             res["data"][i][j] = res["data"][i][j] + a["data"][i][k] * b["data"][k][j];
            //         }
            //     }
            // }
            // return res;

            a["data"] = $matrix_multiply(a["data"], b["data"]);
            return a;
        },
        "rows": rows,
        "cols": cols,
        "data": data
    };
};


let pretty_print_matrix = func(matrix){
    // print "\nMatrix:";
    let padding = 9;
    let width = $vector_length(matrix[0]);
    let temp = "--";
    for (let i = 0; i < width * (padding + 1) - 2; i = i + 1){
        temp = temp + " ";
    }
    temp = temp + "--";
    print temp;

    for (let i = 0; i < $vector_length(matrix); i = i + 1){
        let temp = "|";
        for (let j = 0; j < $vector_length(matrix[i]); j = j + 1){
            let len = $string_length(matrix[i][j] + "");
            for (let k = 0; k < padding - len; k = k + 1){
                temp = temp + " ";
            }
            if (j == $vector_length(matrix[i]) - 1){
                temp = temp + matrix[i][j] + " |";
                print temp;
            }
            else{
                temp = temp + matrix[i][j] + " ";
            }
        }
    }

    temp = "--";
    for (let i = 0; i < width * (padding + 1) - 2; i = i + 1){
        temp = temp + " ";
    }
    temp = temp + "--";
    print temp;

    print "";
    return 0;
};

// let m1 = new_matrix(2, 2, [[1, 2], [3, 4]]);
// let m2 = new_matrix(2, 2, [[5, 6], [7, 8]]);

// print "m1";
// let res = pretty_print_matrix(m1["data"]);
// print "m2";
// let res = pretty_print_matrix(m2["data"]);

// let m3 = m1 + m2;
// let m4 = m1 - m2;
// let m5 = m1 * m2;

// print "m1 + m2";
// let res = pretty_print_matrix(m3["data"]);
// print "m1 - m2";
// let res = pretty_print_matrix(m4["data"]);
// print "m1 * m2";
// let res = pretty_print_matrix(m5["data"]);



// let m6 = m3 * m4 - m5;
// print "m6";
// let res = pretty_print_matrix(m6["data"]);


// print 2 * [0];
// print 2 * [2 * [0]];


// make a random n x m matrix
let random_matrix = func(new_matrix, n, m) {
    let res = new_matrix(n, m, n * [0]);
    for (let i = 0; i < n; i = i + 1) {
        res["data"][i] = m * [0];
        for (let j = 0; j < m; j = j + 1) {
            res["data"][i][j] = $random_int(-5, 5);
        }
    }
    return res;
};

let A = random_matrix(new_matrix, 3, 3);
let x = random_matrix(new_matrix, 3, 1);

let n = 10;
let A = random_matrix(new_matrix, n, n);
let x = random_matrix(new_matrix, n, n);

print "A";
let res = pretty_print_matrix(A["data"]);

print "x";
let res = pretty_print_matrix(x["data"]);

let b = A * x;
print "b";
let res = pretty_print_matrix(b["data"]);

// let A = new_matrix(3, 3, [[1, 2, 3],
//                           [4, 5, 6],
//                           [7, 8, 9]
//                          ]);

// let x = new_matrix(3, 1, [[1],
//                           [2],
//                           [3]]);

// let b = A * x;


// let res = pretty_print_matrix(A["data"]);
// let res = pretty_print_matrix(x["data"]);
// let res = pretty_print_matrix(b["data"]);

