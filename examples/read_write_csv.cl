let mat = [
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
];

$csv_write("mat.csv", mat);

let mat2 = $csv_read("mat.csv");

print mat2;
