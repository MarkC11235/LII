// Vector functions

let vec = [1, 2, 3, 4, 5];
print $vector_len(vec); // Output: 5

vec = $vector_push(vec, 6);
print $vector_len(vec); // Output: 6
print vec; // Output: [1, 2, 3, 4, 5, 6]

vec = $vector_pop(vec);
print $vector_len(vec); // Output: 5
print vec; // Output: [1, 2, 3, 4, 5]

print $vector_insert(vec, 4, 6); // Output: [1, 2, 3, 4, 6, 5]
print $vector_remove(vec, 1); // Output: [1, 3, 4, 5]

print $vector_clear(vec); // Output: []

print $vector_get(vec, 4); // Output: 5
print $vector_set(vec, 4, 6); // Output: [1, 2, 3, 4, 6]

print $vector_slice(vec, 1, 3); // Output: [2, 3]

print $vector_reverse(vec); // Output: [5, 4, 3, 2, 1]

print $vector_concat(vec, vec); // Output: [1, 2, 3, 4, 5, 1, 2, 3, 4, 5]


