let f = func(x){
    return 1;
};

let mf = map {
    "key1" : f,
    f : "value2"
};

print mf[mf["key1"]];

