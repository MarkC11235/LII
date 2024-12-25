let file_path = "foobar.txt";

let lines = [
    "Hello, World!",
    "This is a test.",
    "This is only a test."
];

$file_write_lines(file_path, lines);

let lines = $file_read_lines(file_path);

for(let i = 0; i < $vector_len(lines); i = i + 1){
    print lines[i];
}