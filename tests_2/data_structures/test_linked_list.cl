let Node = map{
    "value" : -1,
    "next" : null
};

let add_back_LL = func(add_back_LL, Node, head, value){
    if(head["next"] == null){
        let node = Node;
        node["value"] = value;
        head["next"] = node;
    } else {
        head["next"] = add_back_LL(add_back_LL, Node, head["next"], value);
    }

    return head;
};

let add_front_LL = func(Node, head, value){
    let node = Node;
    node["value"] = value;
    node["next"] = head;
    return node;
};


let print_LL = func(print_LL, Node, head){
    print head["value"];
    if(head["next"] != null){
        let res = print_LL(print_LL, Node, head["next"]);
    }
    return 0;
};


let head = Node;
head["value"] = 0;

head = add_back_LL(add_back_LL, Node, head, 1);
head = add_back_LL(add_back_LL, Node, head, 2);
head = add_back_LL(add_back_LL, Node, head, 3);

head = add_front_LL(Node, head, -1);
head = add_front_LL(Node, head, -2);
head = add_front_LL(Node, head, -3);

let res = print_LL(print_LL, Node, head);
