let http_req = struct{
    let method = "GET";
    let path = "/";
    let version = "HTTP/1.1";
}; 
let res = $send_request("127.0.0.1", 8080, http_req);
print res;