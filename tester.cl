// let x = 5;
// const y = 10;
// global z = 15;


// let get_global = func(){
//     return z;
// };

// print get_global();

// // attempt to modify global variable
// // z = 20; // will throw an error

// // attempt to modify const variable
// // y = 25; // will throw an error

let set = map {
    "__type": "set",
    "set_insert": func(a, b){
        for (let i = 0; i < $vector_length(a["data"]); i = i + 1) {
            if (a["data"][i] == b) {
                return a;
            }
        }
        a["data"] = $vector_push(a["data"], b);
        return a;
    },
    "data": []
};

define "!" in "set" as func(a){
    let res = a;
    res["data"] = [];
    return res;
};

define "&&" in "set" as func(a, b){
    let res = !a;
    // res["data"] = []; // clear res
    for (let i = 0; i < $vector_length(b["data"]); i = i + 1) {
        let found = false;
        for (let j = 0; j < $vector_length(a["data"]); j = j + 1) {
            if (a["data"][j] == b["data"][i]) {
                found = true;
                break;
            }
        }
        if (found) {
            res["data"] = $vector_push(res["data"], b["data"][i]);
        }
    }
    return res;
};

define "||" in "set" as func(a, b){
    let res = a;
    for (let i = 0; i < $vector_length(b["data"]); i = i + 1) {
        res = a["set_insert"](res, b["data"][i]);
    }
    return res;
};



let a = set;
a = set["set_insert"](a, 1);
a = set["set_insert"](a, 2);
a = set["set_insert"](a, 3);

let b = set;
b = set["set_insert"](b, 2);
b = set["set_insert"](b, 3);
b = set["set_insert"](b, 4);

let c = a && b;
print c;

let d = a || b;
print d;

let e = !a;
print e;
