let factorial = func(i){
    if(i){
        return i * factorial(i - 1);
    } else {
        return 1;
    }
    return -1; // This line should not be reached, but needs to be here for the JIT engine
};

print factorial(5);