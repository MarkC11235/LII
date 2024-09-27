let vec = [1, 2, 3, 4, 5];

let vec1 = $vector_push(vec, vec);
vec = $vector_push(vec, vec1);
print vec;

vec[5][5][0] = 10;

print vec;
print vec[5][5][0]; // 10
