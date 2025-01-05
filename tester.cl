let obj = map{
    // let a = map{
    //     let d = 4;
    //     let e = [1, 2, 3];
    // };
    // let sd = [4, 5, 6];
    // let b = "hello";
    // let c = null;
    // let f = 3.14;
    // let g = true;
    // let h = false;
    // let i = func(){return 1;};
    // let j = 1;
    // let k = -1.2;
    // let l = .2;
    // let m = -.5;
    let j = [map{let iv = 1; let jv = 2;}];
};

let json_string = $map_to_json(obj);
print json_string;
let obj2 = $json_to_map(json_string);

print obj;
print obj2;
