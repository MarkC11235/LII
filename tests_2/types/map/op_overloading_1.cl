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
    a["data"] = $matrix_multiply(a["data"], b["data"]);
    return a;
};

let m1 = matrix;
m1["rows"] = 2;
m1["cols"] = 2;
m1["data"] = [[1, 2],
               [3, 4]];

let m2 = matrix;
m2["rows"] = 2;
m2["cols"] = 2;
m2["data"] = [[5, 6],
               [7, 8]];


let m3 = m1 + m2;
let m4 = m1 - m2;
let m5 = m1 * m2;

print "m1 + m2";
print m3;
print "m1 - m2";
print m4;
print "m1 * m2";
print m5;
