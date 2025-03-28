let map obj = map{
    "string" : "string",
    "number" : 1,
    "boolean" : true,
    "array" : [1, 2, 3],
    "object" : map{
        "key" : "value"
    },
    "nil" : null
};

let string json_string = $map_to_json(obj);
print json_string;
let map obj2 = $json_to_map(json_string);

print obj;
print obj2;
