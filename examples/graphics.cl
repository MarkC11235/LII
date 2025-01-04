let handle_events = func(events) {
    for (let i = 0; i < $vector_len(events); i = i + 1) {
        let event = events[i];
        if (event == "quit") {
            let res = $close_graphics();
            let res = $exit_program("Quit event");
            return -1; // will never be reached
        }
        if (event == "keydown") {
            return 1;
        }
    }
    return 0;
};

let rect = struct{
    let x = 100;
    let y = 100;
    let w = 200;
    let h = 200;
};

let res = $init_graphics("Test", 800, 600);
if (res != 0) {
    let res = $exit_program("Error, $init_graphics(), code: " + res);
}

for(;;) {
    let events = $get_events();
    let res = handle_events(events);

    if (res == 1) {
        rect = struct{
            let x = $random_number(0, 800);
            let y = $random_number(0, 600);
            let w = 200;
            let h = 200;
        };
    }

    let res = $clear_screen();
    let res = $draw_rect(rect["x"], rect["y"], rect["w"], rect["h"]);
    let res = $update_screen();
    let res = $wait(1 / 60); // 60 fps
}



