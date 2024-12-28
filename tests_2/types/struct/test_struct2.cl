let Person = map{
    let name = "Bob";
    let f = func(){return 1;};
    let l = [1, 2, 3];
    let n = null;
    let s = map{let inner = "hello";};
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

