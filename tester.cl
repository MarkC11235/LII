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

let rect = map{
    "x" : 0,
    "y" : 0,
    "w" : 200,
    "h" : 200
};

let res = $init_graphics("Graph", 800, 600);
if (res != 0) {
    let res = $exit_program("Error, $init_graphics(), code: " + res);
}

for(;;) {
    let events = $get_events();
    let res = handle_events(events);

    if (res == 1) {
        rect = map{
            "x" : rect["x"] + 10,
            "y" : rect["y"] + 10,
            "w" : rect["w"],
            "h" : rect["h"]
        };
    }

    let res = $clear_screen();
    // let res = $draw_rect(rect["x"], rect["y"], rect["w"], rect["h"]);
    // let res = $draw_text("Hello, World!", 10, 10, 20);
    let res = $graph([[0, 5, 10, 15, 20], [0, 1, 4, 9, 16]], 0, 25, 0, 25, "TEST", "X", "Y");
    let res = $update_screen();
    let res = $wait(1 / 60); // 60 fps
}



