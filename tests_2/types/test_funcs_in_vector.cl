
let func_vec = [
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

let a = 10;
let b = 20;

for (let i = 0; i < 4; i = i + 1) {
    let f = func_vec[i];
    print f(a, b);
}