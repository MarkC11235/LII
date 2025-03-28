let map Person = map{
    "name": "John",
    "age": 50
};

let map Car = map{
    "make": "Ford",
    "model": "Mustang",
    "year": 1964
};

print Person + Car; // {"name":"John","age":50,"make":"Ford","model":"Mustang","year":1964}


let vector v1 = [1, 2, 3];
let vector v2 = [4, 5, 6];

print v1 + v2; // [1,2,3,4,5,6]


let map ID = map{
    "name": "John",
    "id": 1
};

let map ID2 = map{
    "name": "Bill",
    "id": 2
};

print Person + ID; // {"name":"John","age":50,"id":1}
print Person + ID2; // {"name":"Bill","age":50,"id":2}
print ID + ID2; // {"name":"Bill","id":2}