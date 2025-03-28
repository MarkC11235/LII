let bool x = true;
let bool y = false;

print x && !y || x; // true
print x || y; // true
print !x; // false
print !y; // true

if (x || y) {
    print "x || y";
} 