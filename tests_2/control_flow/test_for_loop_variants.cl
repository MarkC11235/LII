// regular for loop
for(let i = 0 ; i < 10; i = i + 1) {
    print i;
}
print "";

// while implemented as for loop
let i = 0;
for( ; i < 10; ) {
    print i;
    i = i + 1;
}
print "";

// empty for loop body
for(let i = 0 ; i < 10; i = i + 1) {
}
print "done with empty for loop";
