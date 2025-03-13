let matrix = map {
    "__type": "matrix",
    "rows": 0,
    "cols": 0,
    "data": [[]]
};

define "*" in "matrix" as func(m1, m2){
    let r = m1["rows"];
    let c = m1["cols"]; 

    let result = m1;
    result["data"] = r * [c * [0]];
    result["rows"] = r;
    result["cols"] = c;

    for (let i = 0; i < r; i = i + 1){
        for (let j = 0; j < c; j = j + 1){
            for (let k = 0; k < c; k = k + 1){
                result["data"][i][j] = result["data"][i][j] + m1["data"][i][k] * m2["data"][k][j];
            }
        }
    }
    
    return result;
};

let hilbert_matrix = func(matrix, n) {
    let matrix = map {
        "__type": "matrix",
        "rows": n,
        "cols": n,
        "data": n * [n * [0]]
    };

    for (let i = 0; i < n; i = i + 1){
        for (let j = 0; j < n; j = j + 1){
            matrix["data"][i][j] = 1 / (i + j + 1);
        }
    }

    return matrix;
};

print hilbert_matrix(matrix, 3);
