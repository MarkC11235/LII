let vec = [1, 2, 3, 4, 5];
let vec1 = vec;

vec = $vector_push(vec, vec1);

vec[0][1] = 10;

print vec[0][1]; // 10
