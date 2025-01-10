let obj = map{
    "vec" : [
        1,
        "string",
        true,
        null,
        map{
            "key" : "value"
        }
    ],
    "m" : map{
        "number" : 1,
        "string" : "string",
        "boolean" : true,
        "array" : [1, 2, 3],
        "object" : map{
            "key" : "value"
        },
        "nil" : null
    }
};

let json_string = $map_to_json(obj);
print json_string;
let obj2 = $json_to_map(json_string);

print obj;
print obj2;
