let obj = map{
    let vec = [
        1,
        "string",
        true,
        null,
        map{
            let key = "value";
        }
    ];
    let m = map{
        let number = 1;
        let string = "string";
        let boolean = true;
        let array = [1, 2, 3];
        let object = map{
            let key = "value";
        };
        let nil = null;
    };
};

let json_string = $map_to_json(obj);
print json_string;
let obj2 = $json_to_map(json_string);

print obj;
print obj2;
