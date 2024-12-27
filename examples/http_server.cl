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
    print req;
    if($map_size(req) == 0){
        print "No request";
    }
    else{
        let body = "Hello ";
        if(req["path"] == "/"){
            body = body + "World";
        }
        else{
            body = body + "Hackerman";
        }
        let response = struct{
            let status = "HTTP/1.1 200 OK";
            let content_type = "Content-Type: text/html";
            let content_length = "Content-Length: " + $string_len(body);
            let body = body;
        };
        let res = $push_response(req["client_fd"], response);
        print res;
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
