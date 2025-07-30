let func mat_cons = func(){
    let vector mat = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ];
    mat[1][1] = 10;
    return mat[1][1];
};

print mat_cons();