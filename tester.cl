let factorial = func(i){
    if(i){
        return i * factorial(i - 1);
    } else {
        return 1;
    }
    return 0;
};

print factorial(5);