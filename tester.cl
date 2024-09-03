let create_vector = func(size) {
    let vec = [];
    for (let i = 0; i < size; i = i + 1){ 
        vec = $vector_push(vec, i);
    }
    return vec;
};

for(let i = 0; i < 10; i = i + 1){
    let vec = create_vector(10);
    print(vec);
}

