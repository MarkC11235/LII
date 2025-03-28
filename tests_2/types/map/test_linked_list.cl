let map Node = map {
    "value" : 0,
    "next" : -1
};

let map head = Node;
print head;

for (let number i = 0; i < 10; i = i + 1) {
    let map node = Node;
    node["value"] = i;
    node["next"] = head;
    head = node;
}

print head["next"]["value"];
head["next"]["value"] = 10;

for (let map i = head; i["next"] != -1; i = i["next"]) {
    print i["value"];
}