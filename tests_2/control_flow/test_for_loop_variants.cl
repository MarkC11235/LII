// regular for loop
for(let number i = 0 ; i < 10; i = i + 1) {
    print i;
}
print "";

// while implemented as for loop
let number i = 0;
for( ; i < 10; ) {
    print i;
    i = i + 1;
}
print "";

// empty for loop body
for(let number i = 0 ; i < 10; i = i + 1) {
}
print "done with empty for loop";
