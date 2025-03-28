let func pprint = func(m){
    print "x: " + m["x"] + ", y: " + m["y"];
    return 0;
};

let number res = pprint(map{"x" : 1, "y" : 2});