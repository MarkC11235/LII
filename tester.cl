let matrix = map {
    "__type": "matrix",
    "rows": 0,
    "cols": 0,
    "data": []
};

define "+" in "matrix" as func(a, b){
    let a_rows = a["rows"];
    let a_cols = a["cols"];
    let b_rows = b["rows"];
    let b_cols = b["cols"];
    if (a_rows != b_rows || a_cols != b_cols) {
        print "Error: incompatible matrix sizes for addition";
        return null; // BECAUSE NULL CAN'T BE OPERATED ON
    }
    for (let i = 0; i < a_rows; i = i + 1) {
        for (let j = 0; j < a_cols; j = j + 1) {
            a["data"][i][j] = a["data"][i][j] + b["data"][i][j];
        }
    }
    return a;
};

define "-" in "matrix" as func(a, b){
    let a_rows = a["rows"];
    let a_cols = a["cols"];
    let b_rows = b["rows"];
    let b_cols = b["cols"];
    if (a_rows != b_rows || a_cols != b_cols) {
        print "Error: incompatible matrix sizes for subtraction";
        return null; // BECAUSE NULL CAN'T BE OPERATED ON
    }
    for (let i = 0; i < a_rows; i = i + 1) {
        for (let j = 0; j < a_cols; j = j + 1) {
            a["data"][i][j] = a["data"][i][j] - b["data"][i][j];
        }
    }
    return a;
};

define "*" in "matrix" as func(a, b){
    // let a_rows = a["rows"];
    // let a_cols = a["cols"];
    // let b_rows = b["rows"];
    // let b_cols = b["cols"];
    // if (a_cols != b_rows) {
    //     print "Error: incompatible matrix sizes for multiplication";
    //     return null; // BECAUSE NULL CAN'T BE OPERATED ON
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

let m1 = matrix;
m1["rows"] = 3;
m1["cols"] = 3;
m1["data"] = [[1, 2, 3],
               [4, 5, 6],
               [7, 8, 9]];

let m2 = matrix;
m2["rows"] = 3;
m2["cols"] = 3;
m2["data"] = [[9, 8, 7],
               [6, 5, 4],
               [3, 2, 1]];

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
// let random_matrix = func(new_matrix, n, m) {
//     let res = new_matrix(n, m, n * [0]);
//     for (let i = 0; i < n; i = i + 1) {
//         res["data"][i] = m * [0];
//         for (let j = 0; j < m; j = j + 1) {
//             res["data"][i][j] = $random_int(-5, 5);
//         }
//     }
//     return res;
// };

// let A = random_matrix(new_matrix, 3, 3);
// let x = random_matrix(new_matrix, 3, 1);

// let n = 10;
// let A = random_matrix(new_matrix, n, n);
// let x = random_matrix(new_matrix, n, n);

// print "A";
// let res = pretty_print_matrix(A["data"]);

// print "x";
// let res = pretty_print_matrix(x["data"]);

// let b = A * x;
// print "b";
// let res = pretty_print_matrix(b["data"]);

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



// convention to define a custom type as a var (with the type name as the var name), then define operators for it
// this map can be used as a constructor for the type
// let point = map {
//     "__type": "point",
//     "x": 0,
//     "y": 0,
//     "z": 0
// };

// addition between two maps with the type "3point"
// define "+" in "point" as func(a, b){
//     a["x"] = a["x"] + b["x"];
//     a["y"] = a["y"] + b["y"];
//     a["z"] = a["z"] + b["z"];
//     return a;
// };

// // unary negation for a map with the type "3point"
// define "u-" in "point" as func(a){
//     a["x"] = -a["x"];
//     a["y"] = -a["y"];
//     a["z"] = -a["z"];
//     return a;
// };

// let p1 = point;
// p1["x"] = 1;
// p1["y"] = 2;
// p1["z"] = 3;

// let p2 = point;
// p2["x"] = 4;
// p2["y"] = 5;
// p2["z"] = 6;

// let p3 = p1 + p2;
// // let p4 = -p1;

// let print_point = func(p) {
//     print "x: " + p["x"] + ", y: " + p["y"] + ", z: " + p["z"];
//     return 0;
// };
// let res = print_point(p1);
// res = print_point(p2);
// res = print_point(p3);
// res = print_point(p4);


let m = 2 * [2 * [0]];
print m;
