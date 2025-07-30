let map d = map {
    "__type": "dummy",
    "name": "dumdum_mc_stupidhead"
};

define "*" in "dummy" as func(a, b){
    return a;
};

let map d1 = d;
let map d2 = d;

print d1 + d2;

