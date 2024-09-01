for(let i = 0; i < 10; i = i + 1) {
    print i + ": ";
    print " ";
    if(i > 7){
        break;
    }
    if(i > 5){
        continue;
    }
    for(let j = 0; j < 10; j = j + 1) {
        if(j < 5){
            print j;
            continue;
        }
        break;
    }
    print "";
}