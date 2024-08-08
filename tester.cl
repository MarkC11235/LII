let Node = struct {
    let value = 0;
    let next = -1;
};

let head = Node;

let n1 = Node;
n1.value = 1;

head.next = n1;

let n2 = Node;
n2.value = 2;

head.next.next = n2;

print head;