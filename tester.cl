let new_point = func(x, y, z) {
    return map{
        "__type": "3point",
        "__add" : func(a, b) {
            a["x"] = a["x"] + b["x"];
            a["y"] = a["y"] + b["y"];
            a["z"] = a["z"] + b["z"];
            return a;
        },
        "__sub" : func(a, b) {
            a["x"] = a["x"] - b["x"];
            a["y"] = a["y"] - b["y"];
            a["z"] = a["z"] - b["z"];
            return a;
        },
        "x": x,
        "y": y,
        "z": z
    };
};

let print_point = func(p) {
    print "x: " + p["x"] + ", y: " + p["y"] + ", z: " + p["z"];
    return 0;
};

let p1 = new_point(1, 2, 3);
let p2 = new_point(4, 5, 6);
let p3 = p1 + p2;

let res = print_point(p1);
res = print_point(p2);
res = print_point(p3);

let p4 = p1 - p2;
res = print_point(p4);

