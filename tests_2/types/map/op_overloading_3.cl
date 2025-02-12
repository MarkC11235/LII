let fraction = map {
    "__type": "fraction",
    "numerator": 0,
    "denominator": 1
};

// reduce fraction
define "!" in "fraction" as func(a){
    let gcd = func(gcd, a, b){
        if (b == 0) {
            return a;
        }
        return gcd(gcd, b, a % b);
    };
    let g = gcd(gcd, a["numerator"], a["denominator"]);
    a["numerator"] = a["numerator"] / g;
    a["denominator"] = a["denominator"] / g;
    return a;
};

define "+" in "fraction" as func(a, b){
    a["numerator"] = a["numerator"] * b["denominator"] + b["numerator"] * a["denominator"];
    a["denominator"] = a["denominator"] * b["denominator"];
    return !a;
};

define "-" in "fraction" as func(a, b){
    a["numerator"] = a["numerator"] * b["denominator"] - b["numerator"] * a["denominator"];
    a["denominator"] = a["denominator"] * b["denominator"];
    return !a;
};

define "*" in "fraction" as func(a, b){
    a["numerator"] = a["numerator"] * b["numerator"];
    a["denominator"] = a["denominator"] * b["denominator"];
    return !a;
};

define "/" in "fraction" as func(a, b){
    a["numerator"] = a["numerator"] * b["denominator"];
    a["denominator"] = a["denominator"] * b["numerator"];
    return !a;
};

define "==" in "fraction" as func(a, b){
    return a["numerator"] * b["denominator"] == b["numerator"] * a["denominator"];
};

define "!=" in "fraction" as func(a, b){
    return a["numerator"] * b["denominator"] != b["numerator"] * a["denominator"];
};

define "<" in "fraction" as func(a, b){
    return a["numerator"] * b["denominator"] < b["numerator"] * a["denominator"];
};

define "<=" in "fraction" as func(a, b){
    return a["numerator"] * b["denominator"] <= b["numerator"] * a["denominator"];
};

define ">" in "fraction" as func(a, b){
    return a["numerator"] * b["denominator"] > b["numerator"] * a["denominator"];
};

define ">=" in "fraction" as func(a, b){
    return a["numerator"] * b["denominator"] >= b["numerator"] * a["denominator"];
};

let a = fraction;
a["numerator"] = 1;
a["denominator"] = 2;

let b = fraction;
b["numerator"] = 1;
b["denominator"] = 3;

let c = a + b;
let d = a - b;
let e = a * b;
let f = a / b;

print c;
print d;
print e;
print f;

let g = fraction;
g["numerator"] = 5;
g["denominator"] = 10;
print g;
print !g;


let h = fraction;
h["numerator"] = 1;
h["denominator"] = 4;
let i = fraction;
i["numerator"] = 1;
i["denominator"] = 2;
print h == i;
print h != i;
print h < i;
print h <= i;
print h > i;
print h >= i;
