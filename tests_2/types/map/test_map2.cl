let Person = map{
    "name" : "Bob",
    "f" : func(){return 1;},
    "l" : [1, 2, 3],
    "n" : null,
    "s" : map{"inner" : "hello"}
};

print Person;

let f = Person["f"];
print f();

let l = Person["l"];
print l;

let n = Person["n"];
print n;

let s = Person["s"];
print s;

