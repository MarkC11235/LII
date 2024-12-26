let res = $start_server(8080);
if(res == 0){
    print "Server started";
}
else{
    print "Server failed to start";
}
// for(let i = 0; i < 10; i = i + 1){
//     let res = $get_request();
//     print(res);
// }

for(let i = 0; i < 10; i = i + 1){
    let req = $get_request();
    print req;
    if($map_size(req) == 0){
        print "No request";
    }
    else{
        let res = $send_response(req["client_fd"], "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 5\r\n\r\nHello");
        break;
    }
    let res = $wait(1);
}

let res = $stop_server();
if(res == 0){
    print "Server stopped";
}
else{
    print "Server failed to stop";
}
