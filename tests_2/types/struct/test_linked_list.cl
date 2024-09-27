let Node = struct {
    let value = 0;
    let next = -1;
};

let head = Node;
print head;

for (let i = 0; i < 10; i = i + 1) {
    let node = Node;
    node["value"] = i;
    node["next"] = head;
    head = node;
}

for (let i = head; i["next"] != -1; i = i["next"]) {
    print i["value"];
}