global x = 5;

let f = func(){
    x = 10; // This will throw an error because global cannot be reassigned
    return x;
};

print f();