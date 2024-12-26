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

for(let i = 0; i < 3; i = i + 1){
    let res = $wait(1);
}

let res = $stop_server();
if(res == 0){
    print "Server stopped";
}
else{
    print "Server failed to stop";
}
