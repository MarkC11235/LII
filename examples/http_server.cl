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
            "name" : req_body["name"],
            "age" : req_body["age"]
        });

        let response = map{
            "version" : "HTTP/1.1",
            "status_code" : 200,
            "status_message" : "OK",
            "headers" : map{
                "Content-Type" : "application/json",
                "Content-Length" : $string_length(body)
            },
            "body" : body
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
