let http_req = map{
    "method" : "GET",
    "path" : "/",
    "version" : "HTTP/1.1",
    "headers" : map{
        "Content-Type" : "application/json"
    },
    // "body" : "{\"name\":\"Mark\", \"age\":25}"
    "body" : $map_to_json(map{
        "name" : "Mark",
        "age" : 25
    })
};

let res = $send_request("127.0.0.1", 8080, http_req);
print res;
print $json_to_map(res["body"]);