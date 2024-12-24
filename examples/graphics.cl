let handle_events = func(events) {
    for (let i = 0; i < $vector_len(events); i = i + 1) {
        let event = events[i];
        if (event == "quit") {
            $close_graphics();
            $exit_program("Quit event");
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
    $exit_program("Error, $init_graphics(), code: " + res);
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

    $clear_screen();
    $draw_rect(rect["x"], rect["y"], rect["w"], rect["h"]);
    $update_screen();
    $wait(1 / 60); // 60 fps
}



