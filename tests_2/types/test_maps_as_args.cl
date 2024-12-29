let pprint = func(m){
    print "x: " + m["x"] + ", y: " + m["y"];
    return 0;
};

let res = pprint(map{let x = 1; let y = 2;});