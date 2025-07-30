let map set = map {
    "__type": "set",
    "set_insert": func(a, b){
        for (let number i = 0; i < $vector_length(a["data"]); i = i + 1) {
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
    let map res = a;
    res["data"] = [];
    return res;
};

define "&&" in "set" as func(a, b){
    let map res = !a;
    // res["data"] = []; // clear res
    for (let number i = 0; i < $vector_length(b["data"]); i = i + 1) {
        let bool found = false;
        for (let number j = 0; j < $vector_length(a["data"]); j = j + 1) {
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
    let map res = a;
    for (let number i = 0; i < $vector_length(b["data"]); i = i + 1) {
        res = a["set_insert"](res, b["data"][i]);
    }
    return res;
};



let map a = set;
a = set["set_insert"](a, 1);
a = set["set_insert"](a, 2);
a = set["set_insert"](a, 3);

let map b = set;
b = set["set_insert"](b, 2);
b = set["set_insert"](b, 3);
b = set["set_insert"](b, 4);

let map c = a && b;
print c;

let map d = a || b;
print d;

let map e = !a;
print e;
