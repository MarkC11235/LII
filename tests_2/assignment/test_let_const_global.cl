let number x = 5;
x = x + 4;
print x; // 9

const number y = 10;
print y + 1; // 11

global number z = 15;
print z; // 15

let func get_global = func(){
    return z;
};
print get_global(); // 15

