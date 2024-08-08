// Binary Search 

let insert = func(head, value) {
    if (head == -1) {
        let node = BinaryTree;
        node.value = value;
        return node;
    }

    if (value < head.value) {
        head.left = insert(head.left, value);
    } else {
        head.right = insert(head.right, value);
    }

    return head;
};

let BinaryTree = struct {
    let value = -1;
    let left = -1;
    let right = -1;
};

let nums = [4, 54, 21, 2, 5, 12, 13, 7];

let head = BinaryTree;

for (let i = 0; i < $vector_length(nums); i = i + 1) {
    head = insert(head, nums[i]);
}

print head;

