let res = $start_server(8080);
if(res == 0){
    print "Server started";
}
else{
    let res = $exit_program("Server failed to start");
}

for(;;){
    if($server_should_close() == true){
        break;
    }

    let req = $pop_request();
    if($map_size(req) == 0){
        // print "No request";
    }
    else{
        print req;
        let req_body = $json_to_map(req["body"]);

        let body = $map_to_json(map{
            let head = "<head><title>Hello, " + req_body["name"] + "!</title></head>";
            let body = "<body><h1>Hello, " + req_body["name"] + "!</h1></body>";
        });

        let response = map{
            let version = "HTTP/1.1";
            let status_code = 200;
            let status_message = "OK";
            let headers = map{
                let content_type = "Content-Type: application/json";
                let content_length = "Content-Length: " + $string_length(body);
            };
            let body = body;
        };
        let res = $push_response(req["client_fd"], response);
    }
    let res = $wait(1);
}

let res = $stop_server();
if(res == 0){
    print "Server stopped";
}
else{
    let res = $exit_program("Server failed to stop");
}
