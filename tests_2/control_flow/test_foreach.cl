let alphabet = map{
    let a = 1;
    let b = 2;
    let c = 3;
};
foreach(let key:value in alphabet) {
    print key + " : " + value;
    alphabet[key] = value + 1;
}
print alphabet;


let vec = [4, 5, 6];
foreach(let index:element in vec) {
    print index + " : " + element;
    vec[index] = element + 1;
}
print vec;