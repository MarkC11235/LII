#"graph_helpers.clh"

let handle_events = func(events) {
    for (let i = 0; i < $vector_length(events); i = i + 1) {
        let event = events[i];
        if (event["type"] == "quit") {
            let res = $close_graphics();
            let res = $exit_program("Quit event");
            return -1; // will never be reached
        }
    }
    return 0;
};

let draw_func = func() {
    let f = func(x) {
        let res = [];
        for (let i = 0; i < $vector_length(x); i = i + 1) {
            res = $vector_push(res, 1/10 * x[i]^3);
        }
        return res;
    };
    let df = func(x) {
        let res = [];
        for (let i = 0; i < $vector_length(x); i = i + 1) {
            res = $vector_push(res, 3/10*x[i]^2);
        }
        return res;
    };
    let d2f = func(x) {
        let res = [];
        for (let i = 0; i < $vector_length(x); i = i + 1) {
            res = $vector_push(res, 6/10*x[i]);
        }
        return res;
    };
    let d3f = func(x) {
        let res = [];
        for (let i = 0; i < $vector_length(x); i = i + 1) {
            res = $vector_push(res, 6/10);
        }
        return res;
    };
    let x_vals = [
                [0, 1, 5, 10, 20, 25, 30, 35, 40, 50, 60, 70, 80, 90, 100],
                [0, 1, 5, 10, 20, 25, 30, 35, 40, 50, 60, 70, 80, 90, 100],
                [0, 1, 5, 10, 20, 25, 30, 35, 40, 50, 60, 70, 80, 90, 100],
                [0, 1, 5, 10, 20, 25, 30, 35, 40, 50, 60, 70, 80, 90, 100]
                ];
    // let x_vals = [
    //             [0, 1, 5, 10, 20, 25, 30],
    //             [0, 1, 5, 10, 20, 25, 30],
    //             [0, 1, 5, 10, 20, 25, 30],
    //             [0, 1, 5, 10, 20, 25, 30]
    //             ];
    let y_vals = [
                f(x_vals[0]),
                df(x_vals[1]),
                d2f(x_vals[2]),
                d3f(x_vals[3])
                ];
    // print "x_vals: " + x_vals;
    // print "y_vals: " + y_vals;

    let min = func(x){
        let res = x[0];
        for (let i = 1; i < $vector_length(x); i = i + 1) {
            if (x[i] < res) {
                res = x[i];
            }
        }
        return res;
    };
    let max = func(x){
        let res = x[0];
        for (let i = 1; i < $vector_length(x); i = i + 1) {
            if (x[i] > res) {
                res = x[i];
            }
        }
        return res;
    };

    let min_x = 100000000;
    let max_x = -100000000;
    let min_y = 100000000;
    let max_y = -100000000;

    for (let i = 0; i < $vector_length(x_vals); i = i + 1) {
        for (let j = 0; j < $vector_length(x_vals[i]); j = j + 1) {
            if (x_vals[i][j] < min_x) {
                min_x = x_vals[i][j];
            }
            if (x_vals[i][j] > max_x) {
                max_x = x_vals[i][j];
            }
        }
    }
    for (let i = 0; i < $vector_length(y_vals); i = i + 1) {
        for (let j = 0; j < $vector_length(y_vals[i]); j = j + 1) {
            if (y_vals[i][j] < min_y) {
                min_y = y_vals[i][j];
            }
            if (y_vals[i][j] > max_y) {
                max_y = y_vals[i][j];
            }
        }
    }
    let res = $graph(x_vals, y_vals, min_x, max_x, min_y, max_y, "f, df, d2f, d3f", "X", "Y");
    return res;
};

let res = start_graphics("f, df, d2f, d3f", 1600, 900, draw_func, handle_events);

