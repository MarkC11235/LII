let func factorial = func(factorial, i){
    if(i){
        return i * factorial(factorial, i - 1);
    } else {
        return 1;
    }
    return -1; // This line should not be reached, but needs to be here for the JIT engine
};

print factorial(factorial, 5);