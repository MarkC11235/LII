// let m = map {
//     "key1" : "value" + 1,
//     "key2" : "value" + 2,
//     "key" + 3 : "value3"
// };

// print m;

// let temp_map = map {
//     "key1" : "value1",
//     "key2" : "value2"
// }; 

// let m2 = map {
//     temp_map : "value1",
//     "key2" : "value2"
// };

// print m2;

// print m2[temp_map];

let f = func(x){
    return 1;
};

let mf = map {
    "key1" : f,
    f : "value2"
};

print mf[mf["key1"]];