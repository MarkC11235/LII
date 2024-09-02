let print_ten = func(x){
    for(let i = 0; i < 10; i = i + 1){
        print x;
    }
    return x;
};

for(let i = 0; i < 10000; i = i + 1){
    print print_ten(i);
}
