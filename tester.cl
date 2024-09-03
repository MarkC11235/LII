// bubble sort 
// takes about 26 seconds to sort 1000 elements with jit enabled
// takes a lot of time without jit enabled, didn't wait for it to finish


let create_vector = func(size) {
    let vec = [];
    for (let i = 0; i < size; i = i + 1){ 
        vec = $vector_push(vec, $random_number(0, 1000));
    }
    return vec;
};



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

let size = 1000;
let vec = [];
for (let i = 0; i < size; i = i + 1){ 
    vec = $vector_push(vec, $random_number(0, 1000));
}
for (let i = 0; i < 1; i = i + 1){
    //let vec = create_vector(size);
    //print(vec);
    let sorted_vec = sort(vec);
    print(sorted_vec);
}
