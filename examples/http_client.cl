let http_req = map{
    let method = "GET";
    let path = "/";
    let version = "HTTP/1.1";
    let headers = map{
        let content_type = "Content-Type: application/json";
    };
    let body = "{\"name\":\"Mark\", \"age\":25}";
}; 
let res = $send_request("127.0.0.1", 8080, http_req);
print res;
print $json_to_map(res["body"]);