let func inc = func(x){
    return x + 1;
};

   // assignment; condition; next value
for(let number i = 0; i < 10; i = i + 1){
    print inc(i);
}

print 11;
