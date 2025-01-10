let str = "import matplotlib.pyplot as plt\nx = [1, 2, 3, 4, 5]\ny = [10, 20, 25, 30, 35]\nplt.plot(x, y)\nplt.title('Sample Plot')\nplt.xlabel('X-axis')\nplt.ylabel('Y-axis')\nplt.savefig('plot.pdf')\nplt.show()";
let file = "tester.py";

let res = $file_write(file, str);
let res = $run_python_file(file);