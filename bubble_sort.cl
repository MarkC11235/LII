let create_vector = func(size) {
    let vec = [];
    for (let i = 0; i < size; i = i + 1){ 
        vec = $vector_push(vec, $random_number(0, 1000));
    }
    return vec;
};

let vec = create_vector(1000);
print(vec);

let sort = func(vec) {
    let n = $vector_len(vec);
    for (let i = 0; i < n; i = i + 1){
        for (let j = i + 1; j < n; j = j + 1){
            if (vec[i] > vec[j]){
                let temp = vec[i];
                vec[i] = vec[j];
                vec[j] = temp;
            }
        }
    }
    return vec;
};

let sorted_vec = sort(vec);
print(sorted_vec);

