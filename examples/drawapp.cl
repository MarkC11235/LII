let handle_events = func(events) {
    for (let i = 0; i < $vector_length(events); i = i + 1) {
        let event = events[i];
        if (event["type"] == "quit") {
            let res = $close_graphics();
            let res = $exit_program("Quit event");
            return -1; // will never be reached
        }
        // if (event["type"] == "keydown") {
        //     return 1;
        // }
        if( event["type"] == "mousebuttondown" ) {
            if( event["button"] == 1 ) {
                
            }
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

let res = $init_graphics("Test", 800, 600);
if (res != 0) {
    let res = $exit_program("Error, $init_graphics(), code: " + res);
}


let is_clicked = false;
let mosue_pos = [0, 0];
let res = $change_color(255, 0, 0);
let color = [0, 0, 0];
for(;;) {
    let events = $get_events();
    // let res = handle_events(events);

    foreach(let i : event in events) {
        // print event;
        if (event["type"] == "quit") {
            let res = $close_graphics();
            let res = $exit_program("Quit event");
            return -1; // will never be reached
        }
        if (event["type"] == "mousedown") {
            if (event["button"] == 1) {
                print "click";
                is_clicked = true;
            }
        }
        if (event["type"] == "mouseup") {
            if (event["button"] == 1) {
                is_clicked = false;
            }
        }
        if (event["type"] == "mousemove") {
            mosue_pos = [event["x"], event["y"]];
        }
        if (event["type"] == "keydown") {
            let res = $change_color($random_int(0, 255), $random_int(0, 255), $random_int(0, 255));
            // if (event["key"] == "q") {
            //     color[0] = color[0] + 10;
            //     let res = $change_color(color[0], color[1], color[2]);
            // }
            // if (event["key"] == "w") {
            //     color[1] = color[1] + 10;
            //     let res = $change_color(color[0], color[1], color[2]);
            // }
            // if (event["key"] == "e") {
            //     color[2] = color[2] + 10;
            //     let res = $change_color(color[0], color[1], color[2]);
            // }

        }
    }

    // if (res == 1) {
    //     rect = map{
    //         "x" : rect["x"] + 10,
    //         "y" : rect["y"] + 10,
    //         "w" : rect["w"],
    //         "h" : rect["h"]
    //     };
    // }

    // let res = $clear_screen();
    // let res = $draw_rect(rect["x"], rect["y"], rect["w"], rect["h"]);



    if (is_clicked) {
        let res = $draw_circle(mosue_pos[0], mosue_pos[1], 35);
    }

    let res = $update_screen();
    let res = $wait(1 / 30); // 30 fps
    // break;
}



