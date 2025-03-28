let x = 5;
const y = 10;
global z = 15;


let get_global = func(){
    return z;
};

print get_global();

// attempt to modify global variable
z = 20; // will throw an error

// attempt to modify const variable
y = 25; // will throw an error