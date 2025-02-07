let is_prime = func(n){
    if(n < 2){
        return false;
    }
    for(let i = 2; i * i <= n; i = i + 1){
        if(n % i == 0){
            return false;
        }
    }
    return true;
};

let x1 = 1;
let x2 = 100000;
print "Primes from " + x1 + " to " + x2 + ":";
for(let i = x1; i <= x2; i = i + 1){
    if(is_prime(i)){
        print i;
    }
}
