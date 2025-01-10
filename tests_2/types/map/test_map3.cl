let Person = map{
    "name" : "Bob"
};

print Person;

let children = ["Bill", "Jill"];

Person["children"] = children;

print Person;