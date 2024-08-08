// Binary Search 

let BinaryTree = struct {
    let value = -1;
    let left = -1;
    let right = -1;
};

let print_tree = func(tree, indent) {
    if (tree == -1) {
        return 0;
    }

    let str = "";
    for (let i = 0; i < indent; i = i + 1) {
        str = str + " ";
    }
    print str + tree.value;

    let res1 = print_tree(tree.left, indent + 4);
    let res2 = print_tree(tree.right, indent + 4);

    return 0;
};

let insert = func(BinaryTree, head, value) {
    if (head == -1) {
        let node = BinaryTree;
        node.value = value;
        return node;
    }

    if (value < head.value) {
        head.left = insert(BinaryTree, head.left, value);
    } else {
        head.right = insert(BinaryTree, head.right, value);
    }

    return head;
};

let find = func(BinaryTree, head, value) {
    if (head == -1) {
        return -1;
    }

    if (head.value == value) {
        return head;
    }

    if (value < head.value) {
        return find(BinaryTree, head.left, value);
    } else {
        return find(BinaryTree, head.right, value);
    }
};

let nums = [];

for (let i = 0; i < 10; i = i + 1) {
    nums = $vector_push(nums, $random_number(0, 100));
}

let head = -1;

for (let i = 0; i < $vector_len(nums); i = i + 1) {
    head = insert(BinaryTree, head, nums[i]);
}

let res = find(BinaryTree, head, 10);

if (res == -1) {
    print 10 + " Not found\n";
} else {
    print 10 + " Found\n";
}

let res = print_tree(head, 0);