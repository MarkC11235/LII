let d = map {
    "__type": "dummy",
    "name": "dumdum_mc_stupidhead"
};

define "*" in "dummy" as func(a, b){
    return a;
};

let d1 = d;
let d2 = d;

print d1 + d2;

