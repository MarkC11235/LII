let Node = struct{
    let value = 0;
    let next = null; // has to be null, won't allow: let next = Node;
};

let head = Node; // makes a deep copy of the struct

let n1 = Node;

n1.value = 1;
n1.next = null;

head.next = n1;

print head.next.value; // 1