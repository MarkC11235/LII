let http_req = map{
    let method = "GET";
    let path = "/";
    let version = "HTTP/1.1";
    let headers = map{
        let User_Agent = "curl/7.64.1";
        let Accept = "*/*";
    };
    let body = "{\"name\":\"Mark\"}";
}; 
let res = $send_request("127.0.0.1", 8080, http_req);
print res;