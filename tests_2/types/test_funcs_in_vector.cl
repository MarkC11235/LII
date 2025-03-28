let vector func_vec = [
    func (a, b) {
        return a + b;
    },
    func (a, b) {
        return a - b;
    },
    func (a, b) {
        return a * b;
    },
    func (a, b) {
        return a / b;
    }
];

let number a = 10;
let number b = 20;

for (let number i = 0; i < 4; i = i + 1) {
    let func f = func_vec[i];
    print f(a, b);
}