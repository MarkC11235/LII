let map Person = map {
    "name" : "",
    "age" : -1 
};

print Person; // {age = -1, name = }

let map mark = Person;
mark["name"] = "Mark";
mark["age"] = 21;
print mark; // {age = 21, name = Mark}

let map john = Person;
john["name"] = "John";
john["age"] = 25;
print john; // {age = 25, name = John}