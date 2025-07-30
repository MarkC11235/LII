let map Person = map{
    "name" : "Bob",
    "f" : func(){return 1;},
    "l" : [1, 2, 3],
    "n" : null,
    "s" : map{"inner" : "hello"}
};

print Person;

let func f = Person["f"];
print f();

let vector l = Person["l"];
print l;

let null n = Person["n"];
print n;

let map s = Person["s"];
print s;

