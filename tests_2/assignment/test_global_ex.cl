global func create_matrix = func(rows, cols, data) {
    return map{
        "__type": "matrix",
        "rows" : rows,
        "cols" : cols,
        "data" : data // 1D array
    };
};

define "+" in "matrix" as func(a, b){
    if (a["rows"] != b["rows"] || a["cols"] != b["cols"]) {
        print "Matrix dimensions do not match for addition.";
        return null;
    }
    for (let number i = 0; i < a["rows"]; i = i + 1) {
        for (let number j = 0; j < a["cols"]; j = j + 1) {
            a["data"][i * a["cols"] + j] = a["data"][i * a["cols"] + j] + b["data"][i * b["cols"] + j];
        }
    }
    return a;
};

define "-" in "matrix" as func(a, b){
    if (a["rows"] != b["rows"] || a["cols"] != b["cols"]) {
        print "Matrix dimensions do not match for subtraction.";
        return null;
    }
    for (let number i = 0; i < a["rows"]; i = i + 1) {
        for (let number j = 0; j < a["cols"]; j = j + 1) {
            a["data"][i * a["cols"] + j] = a["data"][i * a["cols"] + j] - b["data"][i * b["cols"] + j];
        }
    }
    return a;
};

define "*" in "matrix" as func(a, b){
    if (a["cols"] != b["rows"]) {
        print "Matrix dimensions do not match for multiplication.";
        return null;
    }
    let map result = create_matrix(a["rows"], b["cols"], a["rows"] * b["cols"] * [0]);
    for (let number i = 0; i < a["rows"]; i = i + 1) {
        for (let number j = 0; j < b["cols"]; j = j + 1) {
            for (let number k = 0; k < a["cols"]; k = k + 1) {
                result["data"][i * b["cols"] + j] = result["data"][i * b["cols"] + j] + a["data"][i * a["cols"] + k] * b["data"][k * b["cols"] + j];
            }
        }
    }
    return result;
};

let map A = create_matrix(2, 3, [1, 2, 3, 4, 5, 6]);
let map B = create_matrix(2, 3, [6, 5, 4, 3, 2, 1]);
let map C = create_matrix(3, 2, [1, 2, 3, 4, 5, 6]);

print "A + B = " + (A + B);
print "A - B = " + (A - B);
print "A * C = " + (A * C);
print "(A + B) * C = " + ((A + B) * C);