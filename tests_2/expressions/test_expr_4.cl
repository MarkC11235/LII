print func(a, b){return a + b;}(1, 2); // 3

let arr = [[1, 2], [func(x){return x^2;}, 4]];
print arr[0][1]; // 2
print arr[1][0](3); // 9

let f = func(){
    let g = func(y){
        return y;
    };
    return g;
};

print f()(5); // 5

let matrix_builder = func(){
    let matrix = [[1, 2], [3, 4]];
    return matrix;
};

print matrix_builder()[1][0]; // 3

print 4 + -5 * 2 ^ 5 - 27 + [1, 2, 3][1]; // -181

print !(matrix_builder()[1][0] == 3) && true; // false

print -matrix_builder()[1][0] ^ 2; // -9

