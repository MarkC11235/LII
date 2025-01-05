let obj = map{
    let a = map{
        let d = 4;
        let e = [1, 2, 3];
    };
    let b = "hello";
    let c = null;
    let f = 3.14;
    let g = true;
    let h = false;
    let i = func(){return 1;};
};

let json_string = $map_to_json(obj);
let obj2 = $json_to_map(json_string);

print obj;
print obj2;