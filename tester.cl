let add = func(vec1, vec2){
    let res = [];
    for (let i = 0; i < $vector_len(vec1); i = i + 1){
        res = $vector_push(res, vec1[i] + vec2[i]);
    }
    return res;
};

// let vec1 = [1, 2, 3];
// let vec2 = [4, 5, 6];

let res = add([1, 2], [3, 4]);
print res;

// let null_check = func(vec){
//     if (vec == null){
//         print "null";
//         return 0;
//     }
//      else {
//         print "not null";
//         return 1;
//     }
// };

// let vec = null;
// let res = null_check(vec);
