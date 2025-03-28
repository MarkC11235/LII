let x = func(){
    global x = 5; // This will throw an error because global cannot be assigned outside of main
    return x;
};

print x();